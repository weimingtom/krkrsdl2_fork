//---------------------------------------------------------------------------

#if 0
#include <windows.h>
#include "tvpsnd.h"
#define EXPORT(hr) extern "C" __declspec(dllexport) hr __stdcall
#endif
#include "tp_stub.h"
#include <stdio.h>
//---------------------------------------------------------------------------
#if 0
#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	return 1;
}
#endif
#include "WaveIntf.h"
#include "istream_compat.h"
#define byte BYTE
#define __int64 int64_t

//---------------------------------------------------------------------------
#pragma pack(push,1)

struct TTCWFHeader
{
	char mark[6];  // = "TCWF0\x1a"
	BYTE channels; // チャネル数
	BYTE reserved;
	LONG frequency; // サンプリング周波数
	LONG numblocks; // ブロック数
	LONG bytesperblock; // ブロックごとのバイト数
	LONG samplesperblock; // ブロックごとのサンプル数
};
struct TTCWUnexpectedPeak
{
	unsigned short int pos;
	short int revise;
};
struct TTCWBlockHeader  // ブロックヘッダ ( ステレオの場合はブロックが右・左の順に２つ続く)
{
	short int ms_sample0;
	short int ms_sample1;
	short int ms_idelta;
	byte ms_bpred;
	byte ima_stepindex;
	TTCWUnexpectedPeak peaks[6];
};
#pragma pack(pop)


static int AdaptationTable[]= 
{
	230, 230, 230, 230, 307, 409, 512, 614,
	768, 614, 512, 409, 307, 230, 230, 230 
};

static int AdaptCoeff1 [] = 
{	256, 512, 0, 192, 240, 460, 392 
} ;

static int AdaptCoeff2 [] = 
{	0, -256, 0, 64, 0, -208, -232
} ;

static int ima_index_adjust [16] =
{	-1, -1, -1, -1,		// +0 - +3, decrease the step size
	 2,  4,  6,  8,     // +4 - +7, increase the step size
	-1, -1, -1, -1,		// -0 - -3, decrease the step size
	 2,  4,  6,  8,		// -4 - -7, increase the step size
} ;

static int ima_step_size [89] = 
{	7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41, 45, 
	50, 55, 60, 66, 73, 80, 88, 97, 107, 118, 130, 143, 157, 173, 190, 209, 230, 
	253, 279, 307, 337, 371, 408, 449, 494, 544, 598, 658, 724, 796, 876, 963, 
	1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024, 3327, 
	3660, 4026, 4428, 4871, 5358, 5894, 6484, 7132, 7845, 8630, 9493, 10442,
	11487, 12635, 13899, 15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 
	32767
} ;
//---------------------------------------------------------------------------
#if 0
//---------------------------------------------------------------------------
void strcpy_limit(LPWSTR dest, LPWSTR  src, int n)
{
	wcsncpy(dest, src, n-1);
	dest[n-1] = '\0';
}
//---------------------------------------------------------------------------
ITSSStorageProvider *StorageProvider = NULL;
//---------------------------------------------------------------------------
class TCWFModule : public ITSSModule
{
	ULONG RefCount;

public:
	TCWFModule();
	~TCWFModule();

public:
	// IUnknown
	HRESULT __stdcall QueryInterface(REFIID iid, void ** ppvObject);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);
	
	// ITSSModule
	HRESULT __stdcall GetModuleCopyright(LPWSTR buffer, unsigned long buflen );
	HRESULT __stdcall GetModuleDescription(LPWSTR buffer, unsigned long buflen );
	HRESULT __stdcall GetSupportExts(unsigned long index, LPWSTR mediashortname, LPWSTR buf, unsigned long buflen );
	HRESULT __stdcall GetMediaInfo(LPWSTR url, ITSSMediaBaseInfo ** info );
	HRESULT __stdcall GetMediaSupport(LPWSTR url );
	HRESULT __stdcall GetMediaInstance(LPWSTR url, IUnknown ** instance );
};
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
class TCWFDecoder : public ITSSWaveDecoder
{
	ULONG RefCount;
	TTCWFHeader Header; // ヘッダ情報
	IStream *InputStream;
	__int64 StreamPos;
	short int *SamplePos;
	long DataStart;
	long DataSize;
	__int64 Pos;
	long BufferRemain;
	BYTE *BlockBuffer;
	short int *Samples;
	TSSWaveFormat TSSFormat;

public:
	TCWFDecoder();
	~TCWFDecoder();

public:
	// IUnkown
	HRESULT __stdcall QueryInterface(REFIID iid, void ** ppvObject);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	// ITSSWaveDecoder
	HRESULT __stdcall GetFormat(TSSWaveFormat *format);
	HRESULT __stdcall Render(void *buf, unsigned long bufsamplelen,
			unsigned long *rendered, unsigned long *status);
	HRESULT __stdcall SetPosition(unsigned __int64 samplepos);

	// そのほか
	HRESULT Open(wchar_t * url);
	bool ReadBlock(int , int );
};
//---------------------------------------------------------------------------
// TCWFModule インプリメンテーション ########################################
//---------------------------------------------------------------------------
TCWFModule::TCWFModule()
{
	RefCount = 1;
}
//---------------------------------------------------------------------------
TCWFModule::~TCWFModule()
{
}
//---------------------------------------------------------------------------
HRESULT __stdcall TCWFModule::QueryInterface(REFIID iid, void ** ppvObject)
{
	if(!ppvObject) return E_INVALIDARG;

	*ppvObject=NULL;
	if(!memcmp(&iid,&IID_IUnknown,16))
		*ppvObject=(IUnknown*)this;
	else if(!memcmp(&iid,&IID_ITSSModule,16))
		*ppvObject=(ITSSModule*)this;

	if(*ppvObject)
	{
		AddRef();
		return S_OK;
	}
	return E_NOINTERFACE;
}
//---------------------------------------------------------------------------
ULONG __stdcall TCWFModule::AddRef()
{
	return ++RefCount;
}
//---------------------------------------------------------------------------
ULONG __stdcall TCWFModule::Release()
{
	if(RefCount == 1)
	{
		delete this;
		return 0;
	}
	else
	{
		return --RefCount;
	}
}
//---------------------------------------------------------------------------
HRESULT __stdcall TCWFModule::GetModuleCopyright(LPWSTR buffer, unsigned long buflen)
{
	strcpy_limit(buffer, L"TCWF decoder for TVP Sound System (C) 2000 W.Dee <dee@kikyou.info>",
			buflen);
	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TCWFModule::GetModuleDescription(LPWSTR buffer, unsigned long buflen )
{
	strcpy_limit(buffer, L"TVP's Compressed Wave Format decoder (*.tcw)", buflen);
	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TCWFModule::GetSupportExts(unsigned long index, LPWSTR mediashortname,
												LPWSTR buf, unsigned long buflen )
{
	if(index >= 1) return S_FALSE;
	wcscpy(mediashortname, L"TCWF ファイル");
	strcpy_limit(buf, L".tcw", buflen);
	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TCWFModule::GetMediaInfo(LPWSTR url, ITSSMediaBaseInfo ** info )
{
	return E_NOTIMPL;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TCWFModule::GetMediaSupport(LPWSTR url )
{
	return E_NOTIMPL;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TCWFModule::GetMediaInstance(LPWSTR url, IUnknown ** instance )
{
	HRESULT hr;
	TCWFDecoder * decoder = new TCWFDecoder();
	hr = decoder->Open(url);
	if(FAILED(hr))
	{
		delete decoder;
		return hr;
	}

	*instance = (IUnknown*)decoder;

	return S_OK;
}
#endif
class TCWFDecoder : public tTVPWaveDecoder
{
    TTCWFHeader Header;
    IStream *InputStream;
    tjs_int64 StreamPos;
    short int *SamplePos;
    long DataStart;
    long DataSize;
    tjs_int64 Pos;
    long BufferRemain;
    BYTE *BlockBuffer;
    short int *Samples;
    tTVPWaveFormat TSSFormat;

public:
    TCWFDecoder();
    ~TCWFDecoder();

public:
    // ITSSWaveDecoder
    virtual void GetFormat(tTVPWaveFormat & format);
    virtual bool Render(void *buf, tjs_uint bufsamplelen, tjs_uint& rendered);
    virtual bool SetPosition(tjs_uint64 samplepos);

    bool Open(const ttstr & url);
    bool ReadBlock(int , int );
};

class TCWFWaveDecoderCreator : public tTVPWaveDecoderCreator
{
public:
    tTVPWaveDecoder * Create(const ttstr & storagename, const ttstr & extension) {
        TCWFDecoder * decoder = new TCWFDecoder();
        if(!decoder->Open(storagename))
        {
            delete decoder;
            return nullptr;
        }
        return decoder;
    }
};
//---------------------------------------------------------------------------
// TCWFDecoder インプリメンテーション #######################################
//---------------------------------------------------------------------------
TCWFDecoder::TCWFDecoder()
{
#if 0
	RefCount = 1;
#endif
	InputStream = NULL;
	BlockBuffer=NULL;
	Samples=NULL;
}
//---------------------------------------------------------------------------
TCWFDecoder::~TCWFDecoder()
{
	if(InputStream)
	{
		InputStream->Release();
		InputStream = NULL;
	}
	if(BlockBuffer) delete [] BlockBuffer;
	if(Samples) delete [] Samples;
}
//---------------------------------------------------------------------------
#if 0
HRESULT __stdcall TCWFDecoder::QueryInterface(REFIID iid, void ** ppvObject)
{
	if(!ppvObject) return E_INVALIDARG;

	*ppvObject=NULL;
	if(!memcmp(&iid,&IID_IUnknown,16))
		*ppvObject=(IUnknown*)this;
	else if(!memcmp(&iid,&IID_ITSSWaveDecoder,16))
		*ppvObject=(ITSSWaveDecoder*)this;

	if(*ppvObject)
	{
		AddRef();
		return S_OK;
	}
	return E_NOINTERFACE;
}
#endif
//---------------------------------------------------------------------------
#if 0
ULONG __stdcall TCWFDecoder::AddRef()
{
	return ++RefCount;
}
#endif
//---------------------------------------------------------------------------
#if 0
ULONG __stdcall TCWFDecoder::Release()
{
	if(RefCount == 1)
	{
		delete this;
		return 0;
	}
	else
	{
		return --RefCount;
	}
}
#endif
//---------------------------------------------------------------------------
#if 0
HRESULT __stdcall TCWFDecoder::GetFormat(TSSWaveFormat *format)
{
	*format = TSSFormat;

	return S_OK;
}
#endif
//---------------------------------------------------------------------------
void TCWFDecoder::GetFormat(tTVPWaveFormat & format)
{
	format = TSSFormat;
}
//---------------------------------------------------------------------------
#if 0
HRESULT __stdcall TCWFDecoder::Render(void *buf, unsigned long bufsamplelen,
            unsigned long *rendered, unsigned long *status)
#endif
bool TCWFDecoder::Render(void *buf, tjs_uint bufsamplelen, tjs_uint& rendered)
{
	// 展開
	unsigned long n;
	short int *pbuf=(short int*)buf;
	for(n=0;n<bufsamplelen;n++)
	{
		if(BufferRemain<=0)
		{
			int i;
			for(i=0; i<Header.channels; i++)
			{
				if(!ReadBlock(Header.channels, i))
				{
#if 0
					if(rendered)
						*rendered = n;
					if(status)
						*status = 0;
#endif
					rendered = n;
					Pos+=n;
#if 0
					return S_OK;
#endif
					return false;
				}
			}
			SamplePos = Samples;
			BufferRemain = Header.samplesperblock;
		}
		int i = Header.channels;
		while(i--) *(pbuf++)=*(SamplePos++);
		BufferRemain--;
	}

#if 0
	if(rendered) *rendered=n;
	if(status) *status = 1;
#endif
	rendered=n;
	Pos+=n;
#if 0
	return S_OK;
#endif
	return true;
}
//---------------------------------------------------------------------------
#if 0
HRESULT __stdcall TCWFDecoder::SetPosition(unsigned __int64 samplepos)
#endif
bool TCWFDecoder::SetPosition(tjs_uint64 samplepos)
{

	// pos (ms単位) に移動する
	// 現在位置を保存
	LARGE_INTEGER newpos;
	ULARGE_INTEGER result;
	newpos.QuadPart=0;
	InputStream->Seek(newpos,1,&result);

	tjs_int64 bytepossave=newpos.QuadPart;
	tjs_int64 samplepossave=Pos;

	// 新しい位置を特定
	long newbytepos = samplepos / (Header.samplesperblock);
	long remnant = samplepos - newbytepos * (Header.samplesperblock);
	Pos = samplepos;
	newbytepos *= Header.bytesperblock * Header.channels;

	// シーク
	newpos.QuadPart=DataStart+newbytepos;
	InputStream->Seek(newpos,0,&result);
	if(result.QuadPart != (tjs_uint64) newpos.QuadPart)
	{
		// シーク失敗
		newpos.QuadPart=bytepossave;
		InputStream->Seek(newpos,0,&result);
		Pos=samplepossave;
#if 0
		return E_FAIL;
#endif
		return false;
	}
	
	StreamPos=DataStart+newbytepos;

	int i;
	for(i=0; i<Header.channels; i++)
	{
		ReadBlock(Header.channels, i);
	}

	SamplePos = Samples + remnant * Header.channels;
	BufferRemain = Header.samplesperblock - remnant;

#if 0
	return S_OK;
#endif
	return true;
}
//---------------------------------------------------------------------------
#if 0
HRESULT TCWFDecoder::Open(wchar_t * url)
#endif
bool TCWFDecoder::Open(const ttstr & url)
{
	// url で指定された URL を開きます
	InputStream = NULL;

#if 0
	HRESULT hr;

	hr = StorageProvider->GetStreamForRead(url, (IUnknown**)&InputStream);
	if(FAILED(hr))
	{
		InputStream = NULL;
		return hr;
	}
#endif

	InputStream = TVPCreateIStream(url, TJS_BS_READ);
	if(!InputStream)
	{
		return false;
	}

	ULONG read;
	LARGE_INTEGER p;
	p.QuadPart=0;
	InputStream->Seek(p,0,NULL);

	LARGE_INTEGER newpos;
	newpos.QuadPart=0;
	ULARGE_INTEGER result;
	InputStream->Seek(newpos,0,&result);
#if 0
	if(result.QuadPart != (tjs_uint64) newpos.QuadPart) return E_FAIL;
#endif
	if(result.QuadPart != (tjs_uint64) newpos.QuadPart) return false;

	// TCWF0 チェック
	InputStream->Read(&Header, sizeof(Header), &read);
#if 0
	if(read!=sizeof(Header)) return E_FAIL;
	if(memcmp(Header.mark,"TCWF0\x1a", 6)) return E_FAIL; // マーク
#endif
	if(read!=sizeof(Header)) return false;
	if(memcmp(Header.mark,"TCWF0\x1a", 6)) return false; // マーク

	// 現在位置を取得
	newpos.QuadPart=0;
	InputStream->Seek(newpos,1,&result);
	StreamPos=(long)result.QuadPart;
	DataStart=StreamPos;

	// その他、初期化
	BufferRemain=0;
	Pos=0;

#if 0
	ZeroMemory(&TSSFormat, sizeof(TSSFormat));
	TSSFormat.dwSamplesPerSec = Header.frequency;
	TSSFormat.dwChannels = Header.channels;
	TSSFormat.dwBitsPerSample = 16;
	TSSFormat.dwSeekable = 2;
	TSSFormat.ui64TotalSamples = 0;
	TSSFormat.dwTotalTime = 0;

#endif
	memset(&TSSFormat, 0, sizeof(TSSFormat));
	TSSFormat.SamplesPerSec = Header.frequency;
	TSSFormat.Channels = Header.channels;
	TSSFormat.BitsPerSample = 16;
	TSSFormat.BytesPerSample = 2;
	TSSFormat.Seekable = 2;
	TSSFormat.TotalSamples = 0;
	TSSFormat.TotalTime = 0;
	TSSFormat.SpeakerConfig = 0;
	TSSFormat.IsFloat = false;

#if 0
	return S_OK;
#endif
	return true;
}
//---------------------------------------------------------------------------
bool TCWFDecoder::ReadBlock(int numchans, int chan)
{

	// メモリ確保
	if(!BlockBuffer)
		BlockBuffer=new BYTE[Header.bytesperblock];
	if(!this->Samples)
		this->Samples=new short int[Header.samplesperblock * Header.channels];

	short int * Samples = this->Samples + chan;


	// シーク
	LARGE_INTEGER newpos;
	ULARGE_INTEGER result;
	newpos.QuadPart=0;
	InputStream->Seek(newpos,1,&result);
	if((long)result.QuadPart!=StreamPos)
	{
		newpos.QuadPart=StreamPos;
		InputStream->Seek(newpos,0,&result);
		if(result.QuadPart != (tjs_uint64) newpos.QuadPart) return false;
	}


	// ブロックヘッダ読み込み
	TTCWBlockHeader bheader;
	ULONG read;
	InputStream->Read(&bheader,sizeof(bheader),&read);
	StreamPos+=read;
	if(sizeof(bheader)!=read) return false;
	InputStream->Read(BlockBuffer, Header.bytesperblock - sizeof(bheader) , &read);
	StreamPos+=read;
	if((ULONG)Header.bytesperblock- sizeof(bheader)!=read) return false;

	// デコード
	Samples[0*numchans] = bheader.ms_sample0;
	Samples[1*numchans] = bheader.ms_sample1;
	int idelta = bheader.ms_idelta;
	int bpred = bheader.ms_bpred;
	if(bpred>=7) return false; // おそらく同期がとれていない

	int k;
	int p;

	//MS ADPCM デコード
	int predict;
	int bytecode;
	for (k = 2, p = 0 ; k < Header.samplesperblock ; k ++, p++)
	{
		bytecode = BlockBuffer[p] & 0xF ;

	    int idelta_save=idelta;
		idelta = (AdaptationTable [bytecode] * idelta) >> 8 ;
	    if (idelta < 16) idelta = 16;
	    if (bytecode & 0x8) bytecode -= 0x10 ;
	
    	predict = ((Samples [(k - 1)*numchans] * AdaptCoeff1 [bpred]) 
					+ (Samples [(k - 2)*numchans] * AdaptCoeff2 [bpred])) >> 8 ;
 
		int current = (bytecode * idelta_save) + predict;
    
	    if (current > 32767) 
			current = 32767 ;
	    else if (current < -32768) 
			current = -32768 ;
    
		Samples [k*numchans] = (short int) current ;
	};

	//IMA ADPCM デコード
	int step;
	int stepindex = bheader.ima_stepindex;
	int prev = 0;
	int diff;
	for (k = 2, p = 0 ; k < Header.samplesperblock ; k ++, p++)
	{
		bytecode= (BlockBuffer[p]>>4) & 0xF;
		
		step = ima_step_size [stepindex] ;
		int current = prev;
  

		diff = step >> 3 ;
		if (bytecode & 1) 
			diff += step >> 2 ;
		if (bytecode & 2) 
			diff += step >> 1 ;
		if (bytecode & 4) 
			diff += step ;
		if (bytecode & 8) 
			diff = -diff ;

		current += diff ;

		if (current > 32767) current = 32767;
		else if (current < -32768) current = -32768 ;

		stepindex+= ima_index_adjust [bytecode] ;
	
		if (stepindex< 0)  stepindex = 0 ;
		else if (stepindex > 88)	stepindex = 88 ;

		prev = current ;

		int n = Samples[k*numchans];
		n+=current;
		if (n > 32767) n = 32767;
		else if (n < -32768) n = -32768 ;
		Samples[k*numchans] =n;
	};

	// unexpected peak の修正
	int i;
	for(i=0; i<6; i++)
	{
		if(bheader.peaks[i].revise)
		{
			int pos = bheader.peaks[i].pos;
			int n = Samples[pos*numchans];
			n -= bheader.peaks[i].revise;
			if (n > 32767) n = 32767;
			else if (n < -32768) n = -32768 ;
			Samples[pos*numchans] = n;
		}
	}

	return true;
}
//---------------------------------------------------------------------------

static TCWFWaveDecoderCreator creator;

//---------------------------------------------------------------------------
// tTJSNC_WutcwfInternal : wutcwf internal class
//---------------------------------------------------------------------------
class tTJSNC_WutcwfInternal : public tTJSNativeClass
{
public:
	tTJSNC_WutcwfInternal();

	static tjs_uint32 ClassID;

protected:
	tTJSNativeInstance * CreateNativeInstance();
};

//---------------------------------------------------------------------------
// tTJSNC_WutcwfInternal
//---------------------------------------------------------------------------
tjs_uint32 tTJSNC_WutcwfInternal::ClassID = -1;
tTJSNC_WutcwfInternal::tTJSNC_WutcwfInternal() : tTJSNativeClass(TJS_W("WutcwfInternal"))
{
	TJS_BEGIN_NATIVE_MEMBERS(WutcwfInternal)
	TJS_DECL_EMPTY_FINALIZE_METHOD
//----------------------------------------------------------------------
TJS_BEGIN_NATIVE_CONSTRUCTOR_DECL_NO_INSTANCE(/*TJS class name*/WutcwfInternal)
{
	return TJS_S_OK;
}
TJS_END_NATIVE_CONSTRUCTOR_DECL(/*TJS class name*/WutcwfInternal)
//----------------------------------------------------------------------

//----------------------------------------------------------------------
	TJS_END_NATIVE_MEMBERS

} // end of tTJSNC_WutcwfInternal::tTJSNC_WutcwfInternal
//---------------------------------------------------------------------------
tTJSNativeInstance *tTJSNC_WutcwfInternal::CreateNativeInstance()
{
	return NULL;
}
static iTJSDispatch2 * TVPCreateNativeClass_WutcwfInternal(iTJSDispatch2* global)
{
	TVPRegisterWaveDecoderCreator(&creator);
	iTJSDispatch2 *cls = new tTJSNC_WutcwfInternal();
	return cls;
}

static tTVPAtInstallClass TVPInstallClassWutcwfInternal(TJS_W("WutcwfInternal"), TVPCreateNativeClass_WutcwfInternal, TJS_W(""));

// ##########################################################################
//---------------------------------------------------------------------------
#if 0
EXPORT(HRESULT) GetModuleInstance(ITSSModule **out,
	ITSSStorageProvider *provider,
	IStream * config, HWND mainwin)
{
	StorageProvider = provider;
	*out = new TCWFModule();
	return S_OK;
}
#endif
//---------------------------------------------------------------------------
