//---------------------------------------------------------------------------
/*
	Risa []      alias  [kirikiri-3]
	 stands for "Risa Is a Stagecraft Architecture"
	Copyright (C) 2000 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
//!
//!
//---------------------------------------------------------------------------

/*

*/

#include "tjsCommHead.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "MathAlgorithms.h"
#include "PhaseVocoderDSP.h"
#include "RealFFT.h"
#include <string.h>

#include "tjsUtils.h"
#include "tvpgl_ia32_intf.h"
#include "DetectCPU.h"
#ifndef _WIN32
#if defined(__vita__) || defined(__SWITCH__)
#include <simde/simde/simde-common.h>
#undef SIMDE_HAVE_FENV_H
#endif
#include <simde/x86/sse.h>
#endif

extern void InterleaveOverlappingWindow(float * __restrict dest, const float * __restrict const * __restrict src,
	float * __restrict win, int numch, size_t srcofs, size_t len);
extern void DeinterleaveApplyingWindow(float * __restrict dest[], const float * __restrict src,
					float * __restrict win, int numch, size_t destofs, size_t len);
#ifndef _WIN32
extern void InterleaveOverlappingWindow_sse(float * __restrict dest, const float * __restrict const * __restrict src,
					float * __restrict win, int numch, size_t srcofs, size_t len);
extern void DeinterleaveApplyingWindow_sse(float * __restrict dest[], const float * __restrict src,
					float * __restrict win, int numch, size_t destofs, size_t len);
#endif


//---------------------------------------------------------------------------
tRisaPhaseVocoderDSP::tRisaPhaseVocoderDSP(
				unsigned int framesize,
				unsigned int frequency, unsigned int channels) :
					InputBuffer(framesize * 4 * channels),
					OutputBuffer(framesize * 4 * channels)
{
	FFTWorkIp = NULL;
	FFTWorkW = NULL;
	InputWindow = NULL;
	OutputWindow = NULL;
	AnalWork = NULL;
	SynthWork = NULL;
	LastAnalPhase = NULL;
	LastSynthPhase = NULL;

	FrameSize = framesize;
	OverSampling = 8;
	Frequency = frequency;
	Channels = channels;
	InputHopSize = OutputHopSize = FrameSize / OverSampling;

	TimeScale = 1.0;
	FrequencyScale = 1.0;
	RebuildParams = true;

	LastSynthPhaseAdjustCounter = 0;

	try
	{
		AnalWork  = (float **)TJSAlignedAlloc(sizeof(float *) * Channels, 4);
		SynthWork = (float **)TJSAlignedAlloc(sizeof(float *) * Channels, 4);
		for(unsigned int ch = 0; ch < Channels; ch++)
			AnalWork[ch] = NULL, SynthWork[ch] = NULL;
		for(unsigned int ch = 0; ch < Channels; ch++)
		{
			AnalWork[ch]  = (float *)TJSAlignedAlloc(sizeof(float) * (FrameSize), 4);
			SynthWork[ch] = (float *)TJSAlignedAlloc(sizeof(float) * (FrameSize), 4);
		}

		LastAnalPhase = (float **)TJSAlignedAlloc(sizeof(float *) * Channels, 4);
		for(unsigned int ch = 0; ch < Channels; ch++)
			LastAnalPhase[ch] = NULL;
		for(unsigned int ch = 0; ch < Channels; ch++)
		{
			LastAnalPhase[ch] = (float *)TJSAlignedAlloc(sizeof(float) * (FrameSize/2), 4);
			memset(LastAnalPhase[ch], 0, FrameSize/2 * sizeof(float)); // 0 でクリア
		}

		LastSynthPhase = (float **)TJSAlignedAlloc(sizeof(float *) * Channels, 4);
		for(unsigned int ch = 0; ch < Channels; ch++)
			LastSynthPhase[ch] = NULL;
		for(unsigned int ch = 0; ch < Channels; ch++)
		{
			LastSynthPhase[ch] = (float *)TJSAlignedAlloc(sizeof(float) * (FrameSize/2), 4);
			memset(LastSynthPhase[ch], 0, FrameSize/2 * sizeof(float)); // 0 でクリア
		}

		FFTWorkIp = (int *)TJSAlignedAlloc(sizeof(int) * (static_cast<int>(2+sqrt((double)FrameSize/4))), 4);
		FFTWorkIp[0] = FFTWorkIp[1] = 0;
		FFTWorkW = (float *)TJSAlignedAlloc(sizeof(float) * (FrameSize/2), 4);
		InputWindow = (float *)TJSAlignedAlloc(sizeof(float) * FrameSize, 4);
		OutputWindow = (float *)TJSAlignedAlloc(sizeof(float) * FrameSize, 4);
	}
	catch(...)
	{
		Clear();
		throw;
	}

	//
	float *bufp1;
	size_t buflen1;
	float *bufp2;
	size_t buflen2;

	InputBuffer.GetWritePointer(InputBuffer.GetSize(),
							bufp1, buflen1, bufp2, buflen2);
	if(bufp1) memset(bufp1, 0, sizeof(float)*buflen1);
	if(bufp2) memset(bufp2, 0, sizeof(float)*buflen2);

	OutputBuffer.GetWritePointer(OutputBuffer.GetSize(),
							bufp1, buflen1, bufp2, buflen2);
	if(bufp1) memset(bufp1, 0, sizeof(float)*buflen1);
	if(bufp2) memset(bufp2, 0, sizeof(float)*buflen2);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
tRisaPhaseVocoderDSP::~tRisaPhaseVocoderDSP()
{
	Clear();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void tRisaPhaseVocoderDSP::SetTimeScale(float v)
{
	if(TimeScale != v)
	{
		TimeScale = v;
		RebuildParams = true;
		InputHopSize = OutputHopSize = FrameSize / OverSampling;
		OutputHopSize = static_cast<unsigned int>(InputHopSize * TimeScale) & ~1;
	}
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void tRisaPhaseVocoderDSP::SetFrequencyScale(float v)
{
	if(FrequencyScale != v)
	{
		FrequencyScale = v;
		RebuildParams = true;
	}
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void tRisaPhaseVocoderDSP::SetOverSampling(unsigned int v)
{
	if(v == 0)
	{
		if(TimeScale <= 0.2) v = 2;
		else if(TimeScale <= 1.2) v = 4;
		else v = 8;
	}

	if(OverSampling != v)
	{
		OverSampling = v;
		InputHopSize = OutputHopSize = FrameSize / OverSampling;
		OutputHopSize = static_cast<unsigned int>(InputHopSize * TimeScale) & ~1;
		RebuildParams = true;
	}
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void tRisaPhaseVocoderDSP::Clear()
{
	if(AnalWork)
	{
		for(unsigned int ch = 0; ch < Channels; ch++)
			TJSAlignedDealloc(AnalWork[ch]), AnalWork[ch] = NULL;
		TJSAlignedDealloc(AnalWork), AnalWork = NULL;
	}
	if(SynthWork)
	{
		for(unsigned int ch = 0; ch < Channels; ch++)
			TJSAlignedDealloc(SynthWork[ch]), SynthWork[ch] = NULL;
		TJSAlignedDealloc(SynthWork), SynthWork = NULL;
	}
	if(LastAnalPhase)
	{
		for(unsigned int ch = 0; ch < Channels; ch++)
			TJSAlignedDealloc(LastAnalPhase[ch]), LastAnalPhase[ch] = NULL;
		TJSAlignedDealloc(LastAnalPhase), LastAnalPhase = NULL;
	}
	if(LastSynthPhase)
	{
		for(unsigned int ch = 0; ch < Channels; ch++)
			TJSAlignedDealloc(LastSynthPhase[ch]), LastSynthPhase[ch] = NULL;
		TJSAlignedDealloc(LastSynthPhase), LastSynthPhase = NULL;
	}
	TJSAlignedDealloc(FFTWorkIp), FFTWorkIp = NULL;
	TJSAlignedDealloc(FFTWorkW), FFTWorkW = NULL;
	TJSAlignedDealloc(InputWindow), InputWindow = NULL;
	TJSAlignedDealloc(OutputWindow), OutputWindow = NULL;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
size_t tRisaPhaseVocoderDSP::GetInputFreeSize()
{
	return InputBuffer.GetFreeSize() / Channels;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
bool tRisaPhaseVocoderDSP::GetInputBuffer(
	size_t numsamplegranules,
	float * & p1, size_t & p1size,
	float * & p2, size_t & p2size)
{
	size_t numsamples = numsamplegranules * Channels;

	if(InputBuffer.GetFreeSize() < numsamples) return false;

	InputBuffer.GetWritePointer(numsamples, p1, p1size, p2, p2size);

	p1size /= Channels;
	p2size /= Channels;

	InputBuffer.AdvanceWritePos(numsamples);

	return true;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
size_t tRisaPhaseVocoderDSP::GetOutputReadySize()
{
	return OutputBuffer.GetDataSize() / Channels;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
bool tRisaPhaseVocoderDSP::GetOutputBuffer(
	size_t numsamplegranules,
	const float * & p1, size_t & p1size,
	const float * & p2, size_t & p2size)
{
	size_t numsamples = numsamplegranules * Channels;

	if(OutputBuffer.GetDataSize() < numsamples) return false; // 十分な準備済みサンプルがない

	OutputBuffer.GetReadPointer(numsamples, p1, p1size, p2, p2size);

	p1size /= Channels;
	p2size /= Channels;

	OutputBuffer.AdvanceReadPos(numsamples);

	return true;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
tRisaPhaseVocoderDSP::tStatus tRisaPhaseVocoderDSP::Process()
{
#ifndef _WIN32
	bool use_sse =
			(TVPCPUType & TVP_CPU_HAS_MMX) &&
			(TVPCPUType & TVP_CPU_HAS_SSE) &&
			(TVPCPUType & TVP_CPU_HAS_CMOV);
#endif


	// パラメータの再計算の必要がある場合は再計算をする
	if(RebuildParams)
	{
		// 窓関数の計算(ここではVorbis I 窓)
		float recovery_of_loss_of_vorbis_window = 2.0;
				//         1            1         2
				//  2  =  ∫  1dx  /   ∫   vorbis (x) dx
				//         0            0
				// where vobis = vorbis I window function
		float output_volume =
			TimeScale / FrameSize  / sqrt(FrequencyScale) / OverSampling * 2 *
											recovery_of_loss_of_vorbis_window;
		for(unsigned int i = 0; i < FrameSize; i++)
		{
			double x = ((double)i+0.5)/FrameSize;
			double window = sin(M_PI/2*sin(M_PI*x)*sin(M_PI*x));
			InputWindow[i]  = (float)(window);
			OutputWindow[i] = (float)(window *output_volume);
		}

		// そのほかのパラメータの再計算
		OverSamplingRadian = (float)((2.0*M_PI)/OverSampling);
		OverSamplingRadianRecp = (float)(1.0/OverSamplingRadian);
		FrequencyPerFilterBand = (float)((double)Frequency/FrameSize);
		FrequencyPerFilterBandRecp = (float)(1.0/FrequencyPerFilterBand);
		ExactTimeScale = (float)OutputHopSize / InputHopSize;

		// フラグを倒す
		RebuildParams = false;
	}

	// 入力バッファ内のデータは十分か？
	if(InputBuffer.GetDataSize() < FrameSize * Channels)
		return psInputNotEnough; // 足りない

	// 出力バッファの空きは十分か？
	if(OutputBuffer.GetFreeSize() < FrameSize * Channels)
		return psOutputFull; // 足りない

	// これから書き込もうとする OutputBuffer の領域の最後の OutputHopSize サンプル
	// グラニュールは 0 で埋める (オーバーラップ時にはみ出す部分なので)
	{
		float *p1, *p2;
		size_t p1len, p2len;

		OutputBuffer.GetWritePointer(OutputHopSize*Channels,
				p1, p1len, p2, p2len, (FrameSize - OutputHopSize)*Channels);
		memset(p1, 0, p1len * sizeof(float));
		if(p2) memset(p2, 0, p2len * sizeof(float));
	}

	// 窓関数を適用しつつ、入力バッファから AnalWork に読み込む
	{
		const float *p1, *p2;
		size_t p1len, p2len;
		InputBuffer.GetReadPointer(FrameSize*Channels, p1, p1len, p2, p2len);
		p1len /= Channels;
		p2len /= Channels;
#ifndef _WIN32
		if( use_sse )
		{
			DeinterleaveApplyingWindow_sse(AnalWork, p1, InputWindow, Channels, 0, p1len);
			if(p2)
				DeinterleaveApplyingWindow_sse(AnalWork, p2, InputWindow + p1len, Channels, p1len, p2len);
		}
		else
#endif
		{
			DeinterleaveApplyingWindow(AnalWork, p1, InputWindow, Channels, 0, p1len);
			if(p2)
				DeinterleaveApplyingWindow(AnalWork, p2, InputWindow + p1len, Channels, p1len, p2len);
		}
	}

	for(unsigned int ch = 0; ch < Channels; ch++)
	{
		//------------------------------------------------
		//------------------------------------------------
#ifndef _WIN32
		if(use_sse)
		{
			ProcessCore_sse(ch);
		}
		else
#endif
		{
			ProcessCore(ch);
		}
	}

	{
		float *p1, *p2;
		size_t p1len, p2len;

		OutputBuffer.GetWritePointer(FrameSize*Channels, p1, p1len, p2, p2len);
		p1len /= Channels;
		p2len /= Channels;
#ifndef _WIN32
		if( use_sse )
		{
			InterleaveOverlappingWindow_sse(p1, SynthWork, OutputWindow, Channels, 0, p1len);
			if(p2)
				InterleaveOverlappingWindow_sse(p2, SynthWork, OutputWindow + p1len, Channels, p1len, p2len);
		}
		else
#endif
		{
			InterleaveOverlappingWindow(p1, SynthWork, OutputWindow, Channels, 0, p1len);
			if(p2)
				InterleaveOverlappingWindow(p2, SynthWork, OutputWindow + p1len, Channels, p1len, p2len);
		}
	}

	LastSynthPhaseAdjustCounter += LastSynthPhaseAdjustIncrement;
	if(LastSynthPhaseAdjustCounter >= LastSynthPhaseAdjustInterval)
	{

		LastSynthPhaseAdjustCounter = 0;

		for(unsigned int ch = 0; ch < Channels; ch++)
		{
			unsigned int framesize_d2 = FrameSize / 2;
			for(unsigned int i = 0; i < framesize_d2; i++)
			{
				long int n = static_cast<long int>(LastSynthPhase[ch][i] / (2.0*M_PI));
				LastSynthPhase[ch][i] -= static_cast<float>(n * (2.0*M_PI));
			}
		}
	}

	OutputBuffer.AdvanceWritePos(OutputHopSize * Channels);
	InputBuffer.AdvanceReadPos(InputHopSize * Channels);

	return psNoError;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void tRisaPhaseVocoderDSP::ProcessCore(int ch)
{
	unsigned int framesize_d2 = FrameSize / 2;
	float * analwork = AnalWork[ch];
	float * synthwork = SynthWork[ch];

	rdft(FrameSize, 1, analwork, FFTWorkIp, FFTWorkW); // Real DFT
	analwork[1] = 0.0;

	if(FrequencyScale != 1.0)
	{
		for(unsigned int i = 0; i < framesize_d2; i ++)
		{
			float re = analwork[i*2  ];
			float im = analwork[i*2+1];

			float mag = sqrt(re*re + im*im); // mag = √(re^2+im^2)
			float ang = VFast_arctan2(im, re); // ang = atan(im/re)

			float tmp = LastAnalPhase[ch][i] - ang;
			LastAnalPhase[ch][i] = ang; // 今回の値を保存

			tmp -= i * OverSamplingRadian;

			tmp = WrapPi_F1(tmp);

			tmp =  tmp * OverSamplingRadianRecp;

			float freq = (i + tmp) *FrequencyPerFilterBand;


			analwork[i*2  ] = mag;
			analwork[i*2+1] = freq;
		}



		float FrequencyScale_rcp = 1.0f / FrequencyScale;
		for(unsigned int i = 0; i < framesize_d2; i ++)
		{

			float fi = i * FrequencyScale_rcp;

			unsigned int index = static_cast<unsigned int>(fi); // floor
			float frac = fi - index;

			if(index + 1 < framesize_d2)
			{
				synthwork[i*2  ] =
					analwork[index*2  ] +
					frac * (analwork[index*2+2]-analwork[index*2  ]);
				synthwork[i*2+1] =
					FrequencyScale * (
					analwork[index*2+1] +
					frac * (analwork[index*2+3]-analwork[index*2+1]) );
			}
			else if(index < framesize_d2)
			{
				synthwork[i*2  ] = analwork[index*2  ];
				synthwork[i*2+1] = analwork[index*2+1] * FrequencyScale;
			}
			else
			{
				synthwork[i*2  ] = 0.0;
				synthwork[i*2+1] = 0.0;
			}
		}




		for(unsigned int i = 0; i < framesize_d2; i ++)
		{
			float mag  = synthwork[i*2  ];
			float freq = synthwork[i*2+1];

			float tmp = freq * FrequencyPerFilterBandRecp - (float)i;


			tmp =  tmp * OverSamplingRadian;


			tmp += i   * OverSamplingRadian;


			tmp *= ExactTimeScale;


			LastSynthPhase[ch][i] -= tmp;
			float ang = LastSynthPhase[ch][i];


			float c, s;
			VFast_sincos(ang, s, c);
			synthwork[i*2  ] = mag * c;
			synthwork[i*2+1] = mag * s;
		}
	}
	else
	{
		for(unsigned int i = 0; i < framesize_d2; i ++)
		{

			float re = analwork[i*2  ];
			float im = analwork[i*2+1];

			float mag = sqrt(re*re + im*im); // mag = √(re^2+im^2)
			float ang = VFast_arctan2(im, re); // ang = atan(im/re)


			float tmp = LastAnalPhase[ch][i] - ang;
			LastAnalPhase[ch][i] = ang; // 今回の値を保存

			// phase shift
			float phase_shift = i * OverSamplingRadian;


			tmp -= phase_shift;


			tmp = WrapPi_F1(tmp);


			tmp += phase_shift;

			tmp *= ExactTimeScale;

			LastSynthPhase[ch][i] -= tmp;
			ang = LastSynthPhase[ch][i];

			float c, s;
			VFast_sincos(ang, s, c);
			synthwork[i*2  ] = mag * c;
			synthwork[i*2+1] = mag * s;
		}
	}

	// FFT を実行する
	synthwork[1] = 0.0; // synthwork[1] = nyquist freq. power (どっちみち使えないので0に)
	rdft(FrameSize, -1, SynthWork[ch], FFTWorkIp, FFTWorkW); // Inverse Real DFT
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


#ifndef _WIN32
/*
	このソースコードでは詳しいアルゴリズムの説明は行わない。
	基本的な流れはプレーンなC言語版と変わりないので、
	../opt_default/PhaseVocoderDSP_default.cpp を参照のこと。
*/

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void tRisaPhaseVocoderDSP::ProcessCore_sse(int ch)
{
	unsigned int framesize_d2 = FrameSize / 2;
	float * analwork = AnalWork[ch];
	float * synthwork = SynthWork[ch];

	// 丸めモードを設定
	SetRoundingModeToNearest_SSE();

	// FFT を実行する
	rdft(FrameSize, 1, analwork, FFTWorkIp, FFTWorkW); // Real DFT
	analwork[1] = 0.0; // analwork[1] = nyquist freq. power (どっちみち使えないので0に)

	simde__m128 exact_time_scale = simde_mm_load1_ps(&ExactTimeScale);
	simde__m128 over_sampling_radian_v = simde_mm_load1_ps(&OverSamplingRadian);

	if(FrequencyScale != 1.0)
	{
		// ここでは 4 複素数 (8実数) ごとに処理を行う。
		simde__m128 over_sampling_radian_recp = simde_mm_load1_ps(&OverSamplingRadianRecp);
		simde__m128 frequency_per_filter_band = simde_mm_load1_ps(&FrequencyPerFilterBand);
		simde__m128 frequency_per_filter_band_recp = simde_mm_load1_ps(&FrequencyPerFilterBandRecp);

		for(unsigned int i = 0; i < framesize_d2; i += 4)
		{
			// インターリーブ解除 +  直交座標系→極座標系
			simde__m128 aw3120 = *(simde__m128*)(analwork + i*2    );
			simde__m128 aw7654 = *(simde__m128*)(analwork + i*2 + 4);

			simde__m128 re3210 = simde_mm_shuffle_ps(aw3120, aw7654, SIMDE_MM_SHUFFLE(2,0,2,0));
			simde__m128 im3210 = simde_mm_shuffle_ps(aw3120, aw7654, SIMDE_MM_SHUFFLE(3,1,3,1));

			simde__m128 mag = simde_mm_sqrt_ps(simde_mm_add_ps(simde_mm_mul_ps(re3210,re3210), simde_mm_mul_ps(im3210,im3210)));
			simde__m128 ang = VFast_arctan2_F4_SSE(im3210, re3210);

			// 前回の位相との差をとる
			simde__m128 lastp = *(simde__m128*)(LastAnalPhase[ch] + i);
			*(simde__m128*)(LastAnalPhase[ch] + i) = ang;
			ang = simde_mm_sub_ps(lastp, ang);

			// over sampling の影響を考慮する
			simde__m128 i_3210;
			i_3210 = simde_mm_cvtsi32_ss(i_3210, i);
			i_3210 = simde_mm_shuffle_ps(i_3210, i_3210, SIMDE_MM_SHUFFLE(0,0,0,0));
			i_3210 = simde_mm_add_ps( i_3210, PM128(PFV_INIT) );

			simde__m128 phase_shift = simde_mm_mul_ps(i_3210, over_sampling_radian_v);
			ang = simde_mm_sub_ps( ang, phase_shift );

			// unwrapping をする
			ang = Wrap_Pi_F4_SSE(ang);

			// -M_PI～+M_PIを-1.0～+1.0の変位に変換
			ang = simde_mm_mul_ps( ang, over_sampling_radian_recp );

			// tmp をフィルタバンド中央からの周波数の変位に変換し、
			// それにフィルタバンドの中央周波数を加算する
			simde__m128 freq = simde_mm_mul_ps( simde_mm_add_ps(ang, i_3210), frequency_per_filter_band );

			// analwork に値を格納する
			re3210 = mag;
			im3210 = freq;
			simde__m128 im10re10 = simde_mm_movelh_ps(re3210, im3210);
			simde__m128 im32re32 = simde_mm_movehl_ps(im3210, re3210);
			simde__m128 im1re1im0re0 = simde_mm_shuffle_ps(im10re10, im10re10, SIMDE_MM_SHUFFLE(3,1,2,0));
			simde__m128 im3re3im2re2 = simde_mm_shuffle_ps(im32re32, im32re32, SIMDE_MM_SHUFFLE(3,1,2,0));
			*(simde__m128*)(analwork + i*2    ) = im1re1im0re0;
			*(simde__m128*)(analwork + i*2 + 4) = im3re3im2re2;
		}


		//------------------------------------------------
		// 変換
		//------------------------------------------------
		// 周波数軸方向のリサンプリングを行う
		float FrequencyScale_rcp = 1.0f / FrequencyScale;
		for(unsigned int i = 0; i < framesize_d2; i ++)
		{
			// i に対応するインデックスを得る
			float fi = i * FrequencyScale_rcp;

			// floor(x) と floor(x) + 1 の間でバイリニア補間を行う
			unsigned int index = static_cast<unsigned int>(fi); // floor
			float frac = fi - index;

			if(index + 1 < framesize_d2)
			{
				synthwork[i*2  ] =
					analwork[index*2  ] +
					frac * (analwork[index*2+2]-analwork[index*2  ]);
				synthwork[i*2+1] =
					FrequencyScale * (
					analwork[index*2+1] +
					frac * (analwork[index*2+3]-analwork[index*2+1]) );
			}
			else if(index < framesize_d2)
			{
				synthwork[i*2  ] = analwork[index*2  ];
				synthwork[i*2+1] = analwork[index*2+1] * FrequencyScale;
			}
			else
			{
				synthwork[i*2  ] = 0.0;
				synthwork[i*2+1] = 0.0;
			}
		}

		//------------------------------------------------
		// 合成
		//------------------------------------------------

		// 各フィルタバンドごとに変換
		// 基本的には解析の逆変換である
		for(unsigned int i = 0; i < framesize_d2; i += 4)
		{
			// インターリーブ解除
			simde__m128 sw3120 = *(simde__m128*)(synthwork + i*2    );
			simde__m128 sw7654 = *(simde__m128*)(synthwork + i*2 + 4);

			simde__m128 mag  = simde_mm_shuffle_ps(sw3120, sw7654, SIMDE_MM_SHUFFLE(2,0,2,0));
			simde__m128 freq = simde_mm_shuffle_ps(sw3120, sw7654, SIMDE_MM_SHUFFLE(3,1,3,1));

			// i+3 i+2 i+1 i+0 を準備
			simde__m128 i_3210;
			i_3210 = simde_mm_cvtsi32_ss(i_3210, i);
			i_3210 = simde_mm_shuffle_ps(i_3210, i_3210, SIMDE_MM_SHUFFLE(0,0,0,0));
			i_3210 = simde_mm_add_ps(i_3210, PM128(PFV_INIT));

			// 周波数から各フィルタバンドの中央周波数を減算し、
			// フィルタバンドの中央周波数からの-1.0～+1.0の変位
			// に変換する
			simde__m128 ang = simde_mm_sub_ps(simde_mm_mul_ps(freq, frequency_per_filter_band_recp), i_3210);

			// -1.0～+1.0の変位を-M_PI～+M_PIの位相に変換
			ang = simde_mm_mul_ps( ang, over_sampling_radian_v );

			// OverSampling による位相の補正
			ang = simde_mm_add_ps( ang, simde_mm_mul_ps( i_3210, over_sampling_radian_v ) );

			// TimeScale による位相の補正
			ang = simde_mm_mul_ps( ang, exact_time_scale );

			// 前回の位相と加算する
			// ここでも虚数部の符号が逆になるので注意
			ang = simde_mm_sub_ps( *(simde__m128*)(LastSynthPhase[ch] + i), ang );
			*(simde__m128*)(LastSynthPhase[ch] + i) = ang;

			// 極座標系→直交座標系
			simde__m128 sin, cos;
			VFast_sincos_F4_SSE(ang, sin, cos);
			simde__m128 re3210 = simde_mm_mul_ps( mag, cos );
			simde__m128 im3210 = simde_mm_mul_ps( mag, sin );

			// インターリーブ
			simde__m128 im10re10 = simde_mm_movelh_ps(re3210, im3210);
			simde__m128 im32re32 = simde_mm_movehl_ps(im3210, re3210);
			simde__m128 im1re1im0re0 = simde_mm_shuffle_ps(im10re10, im10re10, SIMDE_MM_SHUFFLE(3,1,2,0));
			simde__m128 im3re3im2re2 = simde_mm_shuffle_ps(im32re32, im32re32, SIMDE_MM_SHUFFLE(3,1,2,0));
			*(simde__m128*)(synthwork + i*2    ) = im1re1im0re0;
			*(simde__m128*)(synthwork + i*2 + 4) = im3re3im2re2;
		}
	}
	else
	{
		// 周波数軸方向にシフトがない場合
		// ここでも 4 複素数 (8実数) ごとに処理を行う。
		for(unsigned int i = 0; i < framesize_d2; i += 4)
		{
			// インターリーブ解除 +  直交座標系→極座標系
			simde__m128 aw3120 = *(simde__m128*)(analwork + i*2    );
			simde__m128 aw7654 = *(simde__m128*)(analwork + i*2 + 4);

			simde__m128 re3210 = simde_mm_shuffle_ps(aw3120, aw7654, SIMDE_MM_SHUFFLE(2,0,2,0));
			simde__m128 im3210 = simde_mm_shuffle_ps(aw3120, aw7654, SIMDE_MM_SHUFFLE(3,1,3,1));

			simde__m128 mag = simde_mm_sqrt_ps( simde_mm_add_ps(simde_mm_mul_ps(re3210,re3210), simde_mm_mul_ps(im3210,im3210)) );
			simde__m128 ang = VFast_arctan2_F4_SSE(im3210, re3210);

			// 前回の位相との差をとる
			simde__m128 lastp = *(simde__m128*)(LastAnalPhase[ch] + i);
			*(simde__m128*)(LastAnalPhase[ch] + i) = ang;
			ang = simde_mm_sub_ps( lastp, ang );

			// over sampling の影響を考慮する
			simde__m128 i_3210;
			i_3210 = simde_mm_cvtsi32_ss(i_3210, i);
			i_3210 = simde_mm_shuffle_ps(i_3210, i_3210, SIMDE_MM_SHUFFLE(0,0,0,0));
			i_3210 = simde_mm_add_ps( i_3210, PM128(PFV_INIT) );

			simde__m128 phase_shift = simde_mm_mul_ps( i_3210, over_sampling_radian_v );
			ang = simde_mm_sub_ps( ang, phase_shift );

			// unwrapping をする
			ang = Wrap_Pi_F4_SSE(ang);

			// OverSampling による位相の補正
			ang = simde_mm_add_ps( ang, phase_shift );

			// TimeScale による位相の補正
			ang = simde_mm_mul_ps( ang, exact_time_scale );

			// 前回の位相と加算する
			// ここでも虚数部の符号が逆になるので注意
			ang = simde_mm_sub_ps( *(simde__m128*)(LastSynthPhase[ch] + i), ang );
			*(simde__m128*)(LastSynthPhase[ch] + i) = ang;

			// 極座標系→直交座標系
			simde__m128 sin, cos;
			VFast_sincos_F4_SSE(ang, sin, cos);
			re3210 = simde_mm_mul_ps( mag, cos );
			im3210 = simde_mm_mul_ps( mag, sin );

			// インターリーブ
			simde__m128 im10re10 = simde_mm_movelh_ps(re3210, im3210);
			simde__m128 im32re32 = simde_mm_movehl_ps(im3210, re3210);
			simde__m128 im1re1im0re0 = simde_mm_shuffle_ps(im10re10, im10re10, SIMDE_MM_SHUFFLE(3,1,2,0));
			simde__m128 im3re3im2re2 = simde_mm_shuffle_ps(im32re32, im32re32, SIMDE_MM_SHUFFLE(3,1,2,0));
			*(simde__m128*)(synthwork + i*2    ) = im1re1im0re0;
			*(simde__m128*)(synthwork + i*2 + 4) = im3re3im2re2;
		}
	}

	// FFT を実行する
	synthwork[1] = 0.0; // synthwork[1] = nyquist freq. power (どっちみち使えないので0に)
	rdft_sse(FrameSize, -1, synthwork, FFTWorkIp, FFTWorkW); // Inverse Real DFT
}
#endif
//---------------------------------------------------------------------------
