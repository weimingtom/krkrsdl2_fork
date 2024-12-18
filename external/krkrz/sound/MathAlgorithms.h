//---------------------------------------------------------------------------
/*
	Risa []      alias 3 [kirikiri-3]
	 stands for "Risa Is a Stagecraft Architecture"
	Copyright (C) 2000 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
//! @file
//! @brief 
//---------------------------------------------------------------------------

#ifndef TVP_MATHALGOLITHMS_H
#define TVP_MATHALGOLITHMS_H

#if !defined(_WIN32)
#if defined(__vita__) || defined(__SWITCH__)
#include <simde/simde/simde-common.h>
#undef SIMDE_HAVE_FENV_H
#endif
#endif
#define _USE_MATH_DEFINES
#include <math.h>
#if defined(_WIN32)
#if 0
#if defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
#include <x86intrin.h>
#include "xmmlib.h"
#endif
#endif
#endif
#if !defined(_WIN32)
#include <simde/x86/sse.h>
#include <simde/x86/sse2.h>
#include "xmmlib.h"
#else
//#include "xmmlib.h"
#endif


//---------------------------------------------------------------------------

// 

//---------------------------------------------------------------------------
/**
 * 
 * 
 *
 */
static inline float VFast_arctan2(float y, float x)
{
   static const float coeff_1 = (float)(M_PI/4);
   static const float coeff_2 = 3*coeff_1;
   float angle;
   float abs_y = fabs(y)+(float)1e-10;     // kludge to prevent 0/0 condition
   if (x>=0)
   {
      float r = (x - abs_y) / (x + abs_y);
      angle = coeff_1 - coeff_1 * r;
   }
   else
   {
      float r = (x + abs_y) / (abs_y - x);
      angle = coeff_2 - coeff_1 * r;
   }
   if (y < 0)
     return(-angle);     // negate if in quad III or IV
   else
     return(angle);
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
static inline float VFast_atan2_madd(float a, float b, float c) { return a*b+c; }
static inline float VFast_atan2_nmsub(float a, float b, float c) { return -(a*b-c); }
static inline float VFast_atan2_round(float a) { return (float)((a>0)?(int)(a+0.5):(int)(a-0.5)); }

/**
 *
 *
 */
static inline void VFast_sincos(float v, float &sin, float &cos)
{
	const float  ss1 =  1.5707963235f  ;
	const float  ss2 =  -0.645963615f  ;
	const float  ss3 =  0.0796819754f  ;
	const float  ss4 =  -0.0046075748f ;
	const float  cc1 =  -1.2336977925f ;
	const float  cc2 =  0.2536086171f  ;
	const float  cc3 =  -0.0204391631f ;

	float s1, s2, c1, c2, fixmag1;
	float x1=VFast_atan2_madd(v, (float)(1.0/(2.0*3.1415926536)), (float)(0.0));
	/* q1=x/2pi reduced onto (-0.5,0.5), q2=q1**2 */
	float q1=VFast_atan2_nmsub(VFast_atan2_round(x1), (float)(1.0), x1);
	float q2=VFast_atan2_madd(q1, q1, (float)(0.0));
	s1= VFast_atan2_madd(q1,
			VFast_atan2_madd(q2,
				VFast_atan2_madd(q2,
					VFast_atan2_madd(q2, (float)(ss4),
								(float)(ss3)),
									(float)( ss2)),
							(float)(ss1)),
						(float)(0.0));
	c1= VFast_atan2_madd(q2,
			VFast_atan2_madd(q2,
				VFast_atan2_madd(q2, (float)(cc3),
				(float)(cc2)),
			(float)(cc1)),
		(float)(1.0));

	/* now, do one out of two angle-doublings to get sin & cos theta/2 */
	c2=VFast_atan2_nmsub(s1, s1, VFast_atan2_madd(c1, c1, (float)(0.0)));
	s2=VFast_atan2_madd((float)(2.0), VFast_atan2_madd(s1, c1, (float)(0.0)), (float)(0.0));

	/* now, cheat on the correction for magnitude drift...
	if the pair has drifted to (1+e)*(cos, sin),
	the next iteration will be (1+e)**2*(cos, sin)
	which is, for small e, (1+2e)*(cos,sin).
	However, on the (1+e) error iteration,
	sin**2+cos**2=(1+e)**2=1+2e also,
	so the error in the square of this term
	will be exactly the error in the magnitude of the next term.
	Then, multiply final result by (1-e) to correct */

	/* this works with properly normalized sine-cosine functions, but un-normalized is more */
	fixmag1=VFast_atan2_nmsub(s2,s2, VFast_atan2_nmsub(c2, c2, (float)(2.0)));

	c1=VFast_atan2_nmsub(s2, s2, VFast_atan2_madd(c2, c2, (float)(0.0)));
	s1=VFast_atan2_madd((float)(2.0), VFast_atan2_madd(s2, c2, (float)(0.0)), (float)(0.0));
	cos=VFast_atan2_madd(c1, fixmag1, (float)(0.0));
	sin=VFast_atan2_madd(s1, fixmag1, (float)(0.0));
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/**
 *
 */
static inline float WrapPi_F1(float v)
{
	int rad_unit = static_cast<int>(v*(1.0/M_PI));
	if (rad_unit >= 0) rad_unit += rad_unit&1;
	else rad_unit -= rad_unit&1;
	v -= (float)(M_PI*(double)rad_unit);
	return v;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/**
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
void DeinterleaveApplyingWindow(float * __restrict dest[], const float * __restrict src,
					float * __restrict win, int numch, size_t destofs, size_t len);
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/**
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
void  InterleaveOverlappingWindow(float * __restrict dest,
	const float * __restrict const * __restrict src,
	float * __restrict win, int numch, size_t srcofs, size_t len);
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

#ifndef _WIN32
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
extern _ALIGN16(const tjs_uint32) TVP_VFASTATAN2_C1[4] ;
extern _ALIGN16(const tjs_uint32) TVP_VFASTATAN2_C1_XOR_C2[4] ;
extern _ALIGN16(const float) TVP_VFASTATAN2_E [4] ;
extern _ALIGN16(const float) TVP_VFASTSINCOS_SS1[4] ;
extern _ALIGN16(const float) TVP_VFASTSINCOS_SS2[4] ;
extern _ALIGN16(const float) TVP_VFASTSINCOS_SS3[4] ;
extern _ALIGN16(const float) TVP_VFASTSINCOS_SS4[4] ;
extern _ALIGN16(const float) TVP_VFASTSINCOS_CC1[4] ;
extern _ALIGN16(const float) TVP_VFASTSINCOS_CC2[4] ;
extern _ALIGN16(const float) TVP_VFASTSINCOS_CC3[4] ;
extern _ALIGN16(const float) TVP_V_R_PI[4] ;
extern _ALIGN16(const float) TVP_V_R_2PI[4] ;
extern _ALIGN16(const float) TVP_V_PI[4] ;
extern _ALIGN16(const float) TVP_V_2PI[4] ;
extern _ALIGN16(const tjs_uint32) TVP_V_I32_1[4];


//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/**
 *
 *
 *
 */
static inline simde__m128 VFast_arctan2_F4_SSE(simde__m128 y, simde__m128 x)
{
	simde__m128 abs_y = simde_mm_add_ps(simde_mm_and_ps(y, PM128(PABSMASK)), PM128(TVP_VFASTATAN2_E));
//   float abs_y = fabs(y)+1e-10;     // kludge to prevent 0/0 condition

	simde__m128 x_sign = simde_mm_and_ps(x, PM128(PCS_RRRR));// 0x80000000 if x < 0
	simde__m128 x_mask = simde_mm_cmple_ps(x, PM128(PFV_0)); // 0xffffffff if x <= 0
	simde__m128 abs_y2 = simde_mm_xor_ps(abs_y , x_sign);
	simde__m128 abs_y1 = simde_mm_xor_ps(abs_y2, PM128(PCS_RRRR));
	simde__m128 r      = simde_mm_div_ps(simde_mm_add_ps(x, abs_y1), simde_mm_add_ps(x, abs_y2));
	r             = simde_mm_xor_ps(r, x_sign);
	simde__m128 coeff_1_or_2 = simde_mm_xor_ps(
							simde_mm_and_ps(x_mask, PM128(TVP_VFASTATAN2_C1_XOR_C2)),
							PM128(TVP_VFASTATAN2_C1)); // x<=0?coeff_2:coeff_1
/*
	simde__m128 coeff_1_or_2 = simde_mm_or_ps(
		simde_mm_and_ps   (x_mask, PM128(TVP_VFASTATAN2_C1)),
		simde_mm_andnot_ps(x_mask, PM128(TVP_VFASTATAN2_C2))); // x>=0?coeff_1:coeff_2
*/
	simde__m128 angle  = simde_mm_sub_ps(coeff_1_or_2, simde_mm_mul_ps(PM128(TVP_VFASTATAN2_C1), r));
/*
   if (x>=0)
   {
      float r =    (x - abs_y) / (x + abs_y)  ;
      angle = coeff_1 - coeff_1 * r;
   }
   else
   {
      float r = -( (x + abs_y) / (x - abs_y)  );
      angle = coeff_2 - coeff_1 * r;
   }
*/
	simde__m128 y_sign = simde_mm_and_ps(y, PM128(PCS_RRRR));
	return simde_mm_xor_ps(angle, y_sign);
/*
   if (y < 0)
     return(-angle);     // negate if in quad III or IV
   else
     return(angle);
*/
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/**
 *
 *
 *
 *
 *
 *
 */
static inline void SetRoundingModeToNearest_SSE()
{
	SIMDE_MM_SET_ROUNDING_MODE(SIMDE_MM_ROUND_NEAREST);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/**
 *
 *
 *
 */
static inline void VFast_sincos_F4_SSE2(simde__m128 v, simde__m128 &sin, simde__m128 &cos)
{
	simde__m128 s1, s2, c1, c2, fixmag1;

	simde__m128 x1 = simde_mm_mul_ps(v, PM128(TVP_V_R_2PI));
//	float x1=madd(v, (float)(1.0/(2.0*3.1415926536)), (float)(0.0));

	/* q1=x/2pi reduced onto (-0.5,0.5), q2=q1**2 */
	simde__m128i r0 = simde_mm_cvtps_epi32(x1);
	simde__m128 q1, q2;
	q1 = simde_mm_cvtepi32_ps(r0);
	q1 = simde_mm_sub_ps(x1, q1);

	q2 = simde_mm_mul_ps(q1, q1);

//	float q1=nmsub(round(x1), (float)(1.0), x1); // q1 = x1 - round(x1)
//	float q2=madd(q1, q1, (float)(0.0));

	s1 = simde_mm_add_ps(simde_mm_mul_ps(q2, PM128(TVP_VFASTSINCOS_SS4)), PM128(TVP_VFASTSINCOS_SS3));
		// s1 = (q2 * ss4 + ss3)
	s1 = simde_mm_add_ps(simde_mm_mul_ps(s1, q2), PM128(TVP_VFASTSINCOS_SS2));
		// s1 = (q2 * (q2 * ss4 + ss3) + ss2)
	s1 = simde_mm_add_ps(simde_mm_mul_ps(s1, q2), PM128(TVP_VFASTSINCOS_SS1));
		// s1 = (q2 * (q2 * (q2 * ss4 + ss3) + ss2) + ss1)
	s1 = simde_mm_mul_ps(s1, q1);

//	s1 = q1 * (q2 * (q2 * (q2 * ss4 + ss3) + ss2) + ss1);
//	s1= madd(q1,
//			madd(q2,
//				madd(q2,
//					madd(q2, (float)(ss4),
//								(float)(ss3)),
//									(float)( ss2)),
//							(float)(ss1)),
//						(float)(0.0));


	c1 = simde_mm_add_ps(simde_mm_mul_ps(q2, PM128(TVP_VFASTSINCOS_CC3)), PM128(TVP_VFASTSINCOS_CC2));
		// c1 = (q2 * cc3 + cc2)
	c1 = simde_mm_add_ps(simde_mm_mul_ps(c1, q2), PM128(TVP_VFASTSINCOS_CC1));
		// c1 =  (q2 * (q2 * cc3 + cc2) + cc1 )
	c1 = simde_mm_add_ps(simde_mm_mul_ps(c1, q2), PM128(PFV_1));
//	c1= (q2 *  (q2 * (q2 * cc3 + cc2) + cc1 ) + 1.0);
//	c1= madd(q2,
//			madd(q2,
//				madd(q2, (float)(cc3),
//				(float)(cc2)),
//			(float)(cc1)),
//		(float)(1.0));

	/* now, do one out of two angle-doublings to get sin & cos theta/2 */
	c2 = simde_mm_sub_ps( simde_mm_mul_ps(c1, c1), simde_mm_mul_ps(s1, s1));
//	c2=nmsub(s1, s1, madd(c1, c1, (float)(0.0))); // c2 = (c1*c1) - (s1*s1)
	s2 = simde_mm_mul_ps(simde_mm_mul_ps(s1, c1), PM128(PFV_2));
//	s2=madd((float)(2.0), madd(s1, c1, (float)(0.0)), (float)(0.0)); // s2=2*s1*c1

	/* now, cheat on the correction for magnitude drift...
	if the pair has drifted to (1+e)*(cos, sin),
	the next iteration will be (1+e)**2*(cos, sin)
	which is, for small e, (1+2e)*(cos,sin).
	However, on the (1+e) error iteration,
	sin**2+cos**2=(1+e)**2=1+2e also,
	so the error in the square of this term
	will be exactly the error in the magnitude of the next term.
	Then, multiply final result by (1-e) to correct */

	/* this works with properly normalized sine-cosine functions, but un-normalized is more */
	simde__m128 c2_c2 = simde_mm_mul_ps(c2, c2);
	simde__m128 s2_s2 = simde_mm_mul_ps(s2, s2);
	fixmag1 = simde_mm_sub_ps(simde_mm_sub_ps(PM128(PFV_2), c2_c2), s2_s2);
//	fixmag1=nmsub(s2,s2, nmsub(c2, c2, (float)(2.0))); // fixmag1 = ( 2.0 - c2*c2 ) - s2*s2

	c1 = simde_mm_sub_ps(c2_c2, s2_s2);
//	c1=nmsub(s2, s2, madd(c2, c2, (float)(0.0))); // c1 = c2*c2 - s2*s2
	s1 = simde_mm_mul_ps(simde_mm_mul_ps(s2, c2), PM128(PFV_2));
//	s1=madd((float)(2.0), madd(s2, c2, (float)(0.0)), (float)(0.0));
	cos = simde_mm_mul_ps(c1, fixmag1);
//	cos=madd(c1, fixmag1, (float)(0.0));
	sin = simde_mm_mul_ps(s1, fixmag1);
//	sin=madd(s1, fixmag1, (float)(0.0));
}
static inline void VFast_sincos_F4_SSE(simde__m128 v, simde__m128 &sin, simde__m128 &cos)
{
	simde__m128 s1, s2, c1, c2, fixmag1;

	simde__m128 x1 = simde_mm_mul_ps(v, PM128(TVP_V_R_2PI));
//	float x1=madd(v, (float)(1.0/(2.0*3.1415926536)), (float)(0.0));

	/* q1=x/2pi reduced onto (-0.5,0.5), q2=q1**2 */
	simde__m64 r0 = simde_mm_cvt_ps2pi(x1);
	simde__m64 r1 = simde_mm_cvt_ps2pi(simde_mm_movehl_ps(x1, x1));
	simde__m128 q1, q2;
	q1 = simde_mm_movelh_ps(simde_mm_cvtpi32_ps(q1, r0), simde_mm_cvtpi32_ps(q1, r1));
	q1 = simde_mm_sub_ps(x1, q1);

	q2 = simde_mm_mul_ps(q1, q1);

//	float q1=nmsub(round(x1), (float)(1.0), x1); // q1 = x1 - round(x1)
//	float q2=madd(q1, q1, (float)(0.0));

	s1 = simde_mm_add_ps(simde_mm_mul_ps(q2, PM128(TVP_VFASTSINCOS_SS4)), PM128(TVP_VFASTSINCOS_SS3));
		// s1 = (q2 * ss4 + ss3)
	s1 = simde_mm_add_ps(simde_mm_mul_ps(s1, q2), PM128(TVP_VFASTSINCOS_SS2));
		// s1 = (q2 * (q2 * ss4 + ss3) + ss2)
	s1 = simde_mm_add_ps(simde_mm_mul_ps(s1, q2), PM128(TVP_VFASTSINCOS_SS1));
		// s1 = (q2 * (q2 * (q2 * ss4 + ss3) + ss2) + ss1)
	s1 = simde_mm_mul_ps(s1, q1);

//	s1 = q1 * (q2 * (q2 * (q2 * ss4 + ss3) + ss2) + ss1);
//	s1= madd(q1,
//			madd(q2,
//				madd(q2,
//					madd(q2, (float)(ss4),
//								(float)(ss3)),
//									(float)( ss2)),
//							(float)(ss1)),
//						(float)(0.0));


	c1 = simde_mm_add_ps(simde_mm_mul_ps(q2, PM128(TVP_VFASTSINCOS_CC3)), PM128(TVP_VFASTSINCOS_CC2));
		// c1 = (q2 * cc3 + cc2)
	c1 = simde_mm_add_ps(simde_mm_mul_ps(c1, q2), PM128(TVP_VFASTSINCOS_CC1));
		// c1 =  (q2 * (q2 * cc3 + cc2) + cc1 )
	c1 = simde_mm_add_ps(simde_mm_mul_ps(c1, q2), PM128(PFV_1));
//	c1= (q2 *  (q2 * (q2 * cc3 + cc2) + cc1 ) + 1.0);
//	c1= madd(q2,
//			madd(q2,
//				madd(q2, (float)(cc3),
//				(float)(cc2)),
//			(float)(cc1)),
//		(float)(1.0));

	/* now, do one out of two angle-doublings to get sin & cos theta/2 */
	c2 = simde_mm_sub_ps( simde_mm_mul_ps(c1, c1), simde_mm_mul_ps(s1, s1));
//	c2=nmsub(s1, s1, madd(c1, c1, (float)(0.0))); // c2 = (c1*c1) - (s1*s1)
	s2 = simde_mm_mul_ps(simde_mm_mul_ps(s1, c1), PM128(PFV_2));
//	s2=madd((float)(2.0), madd(s1, c1, (float)(0.0)), (float)(0.0)); // s2=2*s1*c1

	/* now, cheat on the correction for magnitude drift...
	if the pair has drifted to (1+e)*(cos, sin),
	the next iteration will be (1+e)**2*(cos, sin)
	which is, for small e, (1+2e)*(cos,sin).
	However, on the (1+e) error iteration,
	sin**2+cos**2=(1+e)**2=1+2e also,
	so the error in the square of this term
	will be exactly the error in the magnitude of the next term.
	Then, multiply final result by (1-e) to correct */

	/* this works with properly normalized sine-cosine functions, but un-normalized is more */
	simde__m128 c2_c2 = simde_mm_mul_ps(c2, c2);
	simde__m128 s2_s2 = simde_mm_mul_ps(s2, s2);
	fixmag1 = simde_mm_sub_ps(simde_mm_sub_ps(PM128(PFV_2), c2_c2), s2_s2);
//	fixmag1=nmsub(s2,s2, nmsub(c2, c2, (float)(2.0))); // fixmag1 = ( 2.0 - c2*c2 ) - s2*s2

	c1 = simde_mm_sub_ps(c2_c2, s2_s2);
//	c1=nmsub(s2, s2, madd(c2, c2, (float)(0.0))); // c1 = c2*c2 - s2*s2
	s1 = simde_mm_mul_ps(simde_mm_mul_ps(s2, c2), PM128(PFV_2));
//	s1=madd((float)(2.0), madd(s2, c2, (float)(0.0)), (float)(0.0));
	cos = simde_mm_mul_ps(c1, fixmag1);
//	cos=madd(c1, fixmag1, (float)(0.0));
	sin = simde_mm_mul_ps(s1, fixmag1);
//	sin=madd(s1, fixmag1, (float)(0.0));

	simde_mm_empty();
}
#if 0
#define VFast_sincos_F4_SSE VFast_sincos_F4_SSE2
#endif
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/**
 *
 */
static inline simde__m128 Wrap_Pi_F4_SSE2(simde__m128 v)
{
	//
	simde__m128 v_quant = simde_mm_mul_ps(v, PM128(TVP_V_R_PI)); // v_quant = v/M_PI

	//
	simde__m128i q = simde_mm_cvttps_epi32(v_quant);
	//
	// a = v_quant,    v_quant = a + ( (0 - (a&1)) & ((a>>31)|1) )
	q =
		simde_mm_add_epi32(
			q,
			simde_mm_and_si128(
				simde_mm_sub_epi32(
					simde_mm_setzero_si128(),
					simde_mm_and_si128(q, PM128I(TVP_V_I32_1))
				),
				simde_mm_or_si128(
					simde_mm_srai_epi32(q, 31),
					PM128I(TVP_V_I32_1)
				)
			)
		);
	//
	v_quant = simde_mm_cvtepi32_ps(q);
	v_quant = simde_mm_mul_ps(v_quant, PM128(TVP_V_PI));

	//
	v = simde_mm_sub_ps(v, v_quant);

	//
	return v;
}
static inline simde__m128 Wrap_Pi_F4_SSE(simde__m128 v)
{
	//
	simde__m128 v_quant = simde_mm_mul_ps(v, PM128(TVP_V_R_PI)); // v_quant = v/M_PI

	//
	simde__m64 q0 = simde_mm_cvtt_ps2pi(v_quant); 
	simde__m64 q1 = simde_mm_cvtt_ps2pi(simde_mm_movehl_ps(v_quant, v_quant));

	//
	// a = v_quant,    v_quant = a + ( (0 - (a&1)) & ((a>>31)|1) )

	q0 =
		simde_mm_add_pi32(
			q0,
			simde_mm_and_si64(
				simde_mm_sub_pi32(
					simde_mm_setzero_si64(),
					simde_mm_and_si64(q0, PM64(TVP_V_I32_1))
				),
				simde_mm_or_si64(
					simde_mm_srai_pi32(q0, 31),
					PM64(TVP_V_I32_1)
				)
			)
		);

	q1 =
		simde_mm_add_pi32(
			q1,
			simde_mm_and_si64(
				simde_mm_sub_pi32(
					simde_mm_setzero_si64(),
					simde_mm_and_si64(q1, PM64(TVP_V_I32_1))
				),
				simde_mm_or_si64(
					simde_mm_srai_pi32(q1, 31),
					PM64(TVP_V_I32_1)
				)
			)
		);

	//
	v_quant = simde_mm_movelh_ps(simde_mm_cvtpi32_ps(v, q0), simde_mm_cvtpi32_ps(v, q1));
	v_quant = simde_mm_mul_ps(v_quant, PM128(TVP_V_PI));

	//
	v = simde_mm_sub_ps(v, v_quant);

	//
	simde_mm_empty();

	//
	return v;
}
#if 0
//
#define Wrap_Pi_F4_SSE Wrap_Pi_F4_SSE2
#endif
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
/**
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
void DeinterleaveApplyingWindow_sse(float * __restrict dest[], const float * __restrict src,
					float * __restrict win, int numch, size_t destofs, size_t len);
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/**
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
void  InterleaveOverlappingWindow_sse(float * __restrict dest, const float * __restrict const * __restrict src,
					float * __restrict win, int numch, size_t srcofs, size_t len);
//---------------------------------------------------------------------------
#endif


#endif
