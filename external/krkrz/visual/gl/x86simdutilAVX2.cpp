
/******************************************************************************/
/**
 * 数学関数 sin, cos, exp を SIMD 化したもの
 * ----------------------------------------------------------------------------
 * 	Copyright (C) T.Imoto <http://www.kaede-software.com>
 * ----------------------------------------------------------------------------
 * @author		T.Imoto
 * @date		2014/04/01
 * @note		sse_mathfun を AVX に移植したもの。一部 AVX2 命令を使用している
 *				ので、AVX2 も必要。
 *****************************************************************************/

#if defined(__vita__) || defined(__SWITCH__)
#include <simde/simde/simde-common.h>
#undef SIMDE_HAVE_FENV_H
#endif
#include <simde/x86/avx.h>
#include <simde/x86/avx2.h>

#undef _PS_CONST
#undef _PI32_CONST

#ifdef _MSC_VER /* visual c++ */
# define ALIGN32_BEG __declspec(align(32))
# define ALIGN32_END 
#else /* gcc or icc */
# define ALIGN32_BEG
# define ALIGN32_END __attribute__((aligned(32)))
#endif

#define _PS_CONST256(Name, Val)                                            \
  static const ALIGN32_BEG float m256_ps_##Name[8] ALIGN32_END = { Val, Val, Val, Val, Val, Val, Val, Val }
#define _PI32_CONST256(Name, Val)                                            \
  static const ALIGN32_BEG int m256_pi32_##Name[8] ALIGN32_END = { Val, Val, Val, Val, Val, Val, Val, Val }
#define _PS_CONST_TYPE256(Name, Type, Val)                                 \
  static const ALIGN32_BEG Type m256_ps_##Name[8] ALIGN32_END = { Val, Val, Val, Val, Val, Val, Val, Val }

_PS_CONST256(1  , 1.0f);
_PS_CONST256(0p5, 0.5f);
/* the smallest non denormalized float number */
_PS_CONST_TYPE256(min_norm_pos, int, 0x00800000);
_PS_CONST_TYPE256(sign_mask, int, (const int)0x80000000);
_PS_CONST_TYPE256(inv_sign_mask, int, ~0x80000000);

_PI32_CONST256(1, 1);
_PI32_CONST256(inv1, ~1);
_PI32_CONST256(2, 2);
_PI32_CONST256(4, 4);
_PI32_CONST256(0x7f, 0x7f);

_PS_CONST256(cephes_SQRTHF, 0.707106781186547524f);
_PS_CONST256(cephes_log_p0, 7.0376836292E-2f);
_PS_CONST256(cephes_log_p1, - 1.1514610310E-1f);
_PS_CONST256(cephes_log_p2, 1.1676998740E-1f);
_PS_CONST256(cephes_log_p3, - 1.2420140846E-1f);
_PS_CONST256(cephes_log_p4, + 1.4249322787E-1f);
_PS_CONST256(cephes_log_p5, - 1.6668057665E-1f);
_PS_CONST256(cephes_log_p6, + 2.0000714765E-1f);
_PS_CONST256(cephes_log_p7, - 2.4999993993E-1f);
_PS_CONST256(cephes_log_p8, + 3.3333331174E-1f);
_PS_CONST256(cephes_log_q1, -2.12194440e-4f);



_PS_CONST256(exp_hi,	88.3762626647949f);
_PS_CONST256(exp_lo,	-88.3762626647949f);

_PS_CONST256(cephes_LOG2EF, 1.44269504088896341f);
_PS_CONST256(cephes_exp_C1, 0.693359375f);
_PS_CONST256(cephes_exp_C2, -2.12194440e-4f);

_PS_CONST256(cephes_exp_p0, 1.9875691500E-4f);
_PS_CONST256(cephes_exp_p1, 1.3981999507E-3f);
_PS_CONST256(cephes_exp_p2, 8.3334519073E-3f);
_PS_CONST256(cephes_exp_p3, 4.1665795894E-2f);
_PS_CONST256(cephes_exp_p4, 1.6666665459E-1f);
_PS_CONST256(cephes_exp_p5, 5.0000001201E-1f);

simde__m256 mm256_exp_ps(simde__m256 x) {
  simde__m256 tmp = simde_mm256_setzero_ps(), fx;
  simde__m256i emm0;
  simde__m256 one = *(simde__m256*)m256_ps_1;

  x = simde_mm256_min_ps(x, *(simde__m256*)m256_ps_exp_hi);
  x = simde_mm256_max_ps(x, *(simde__m256*)m256_ps_exp_lo);

  /* express exp(x) as exp(g + n*log(2)) */
  fx = simde_mm256_mul_ps(x, *(simde__m256*)m256_ps_cephes_LOG2EF);
  fx = simde_mm256_add_ps(fx, *(simde__m256*)m256_ps_0p5);

  /* how to perform a floorf with SSE: just below */
  /* step 1 : cast to int */
  emm0 = simde_mm256_cvttps_epi32(fx);
  /* step 2 : cast back to float */
  tmp  = simde_mm256_cvtepi32_ps(emm0);

  /* if greater, substract 1 */
  simde__m256 mask = simde_mm256_cmp_ps( tmp, fx, SIMDE_CMP_GT_OS );
  mask = simde_mm256_and_ps(mask, one);
  fx = simde_mm256_sub_ps(tmp, mask);

  tmp = simde_mm256_mul_ps(fx, *(simde__m256*)m256_ps_cephes_exp_C1);
  simde__m256 z = simde_mm256_mul_ps(fx, *(simde__m256*)m256_ps_cephes_exp_C2);
  x = simde_mm256_sub_ps(x, tmp);
  x = simde_mm256_sub_ps(x, z);

  z = simde_mm256_mul_ps(x,x);
  
  simde__m256 y = *(simde__m256*)m256_ps_cephes_exp_p0;
  y = simde_mm256_mul_ps(y, x);
  y = simde_mm256_add_ps(y, *(simde__m256*)m256_ps_cephes_exp_p1);
  y = simde_mm256_mul_ps(y, x);
  y = simde_mm256_add_ps(y, *(simde__m256*)m256_ps_cephes_exp_p2);
  y = simde_mm256_mul_ps(y, x);
  y = simde_mm256_add_ps(y, *(simde__m256*)m256_ps_cephes_exp_p3);
  y = simde_mm256_mul_ps(y, x);
  y = simde_mm256_add_ps(y, *(simde__m256*)m256_ps_cephes_exp_p4);
  y = simde_mm256_mul_ps(y, x);
  y = simde_mm256_add_ps(y, *(simde__m256*)m256_ps_cephes_exp_p5);
  y = simde_mm256_mul_ps(y, z);
  y = simde_mm256_add_ps(y, x);
  y = simde_mm256_add_ps(y, one);

  /* build 2^n */
  emm0 = simde_mm256_cvttps_epi32(fx);
  emm0 = simde_mm256_add_epi32(emm0, *(simde__m256i*)m256_pi32_0x7f);
  emm0 = simde_mm256_slli_epi32(emm0, 23);
  simde__m256 pow2n = simde_mm256_castsi256_ps(emm0);

  y = simde_mm256_mul_ps(y, pow2n);
#ifdef __AVX__
  _mm256_zeroupper();
#endif
  return y;
}

_PS_CONST256(minus_cephes_DP1, -0.78515625f);
_PS_CONST256(minus_cephes_DP2, -2.4187564849853515625e-4f);
_PS_CONST256(minus_cephes_DP3, -3.77489497744594108e-8f);
_PS_CONST256(sincof_p0, -1.9515295891E-4f);
_PS_CONST256(sincof_p1,  8.3321608736E-3f);
_PS_CONST256(sincof_p2, -1.6666654611E-1f);
_PS_CONST256(coscof_p0,  2.443315711809948E-005f);
_PS_CONST256(coscof_p1, -1.388731625493765E-003f);
_PS_CONST256(coscof_p2,  4.166664568298827E-002f);
_PS_CONST256(cephes_FOPI, 1.27323954473516f); // 4 / M_PI
/**
 * SSE 版のものを AVX でも作る
 */
simde__m256 mm256_sin_ps(simde__m256 x) { // any x
  simde__m256 xmm1, xmm2 = simde_mm256_setzero_ps(), xmm3, sign_bit, y;
  simde__m256i emm0, emm2;
  sign_bit = x;
  /* take the absolute value */
  x = simde_mm256_and_ps(x, *(simde__m256*)m256_ps_inv_sign_mask);
  /* extract the sign bit (upper one) */
  sign_bit = simde_mm256_and_ps(sign_bit, *(simde__m256*)m256_ps_sign_mask);
  
  /* scale by 4/Pi */
  y = simde_mm256_mul_ps(x, *(simde__m256*)m256_ps_cephes_FOPI);

  /* store the integer part of y in mm0 */
  emm2 = simde_mm256_cvttps_epi32(y);
  /* j=(j+1) & (~1) (see the cephes sources) */
  emm2 = simde_mm256_add_epi32(emm2, *(simde__m256i*)m256_pi32_1);
  emm2 = simde_mm256_and_si256(emm2, *(simde__m256i*)m256_pi32_inv1);
  y = simde_mm256_cvtepi32_ps(emm2);
  /* get the swap sign flag */
  emm0 = simde_mm256_and_si256(emm2, *(simde__m256i*)m256_pi32_4);
  emm0 = simde_mm256_slli_epi32(emm0, 29);
  /* get the polynom selection mask 
     there is one polynom for 0 <= x <= Pi/4
     and another one for Pi/4<x<=Pi/2

     Both branches will be computed.
  */
  emm2 = simde_mm256_and_si256(emm2, *(simde__m256i*)m256_pi32_2);
  emm2 = simde_mm256_cmpeq_epi32(emm2, simde_mm256_setzero_si256());
  
  simde__m256 swap_sign_bit = simde_mm256_castsi256_ps(emm0);
  simde__m256 poly_mask = simde_mm256_castsi256_ps(emm2);
  sign_bit = simde_mm256_xor_ps(sign_bit, swap_sign_bit);

  /* The magic pass: "Extended precision modular arithmetic" 
     x = ((x - y * DP1) - y * DP2) - y * DP3; */
  xmm1 = *(simde__m256*)m256_ps_minus_cephes_DP1;
  xmm2 = *(simde__m256*)m256_ps_minus_cephes_DP2;
  xmm3 = *(simde__m256*)m256_ps_minus_cephes_DP3;
  xmm1 = simde_mm256_mul_ps(y, xmm1);
  xmm2 = simde_mm256_mul_ps(y, xmm2);
  xmm3 = simde_mm256_mul_ps(y, xmm3);
  x = simde_mm256_add_ps(x, xmm1);
  x = simde_mm256_add_ps(x, xmm2);
  x = simde_mm256_add_ps(x, xmm3);

  /* Evaluate the first polynom  (0 <= x <= Pi/4) */
  y = *(simde__m256*)m256_ps_coscof_p0;
  simde__m256 z = simde_mm256_mul_ps(x,x);

  y = simde_mm256_mul_ps(y, z);
  y = simde_mm256_add_ps(y, *(simde__m256*)m256_ps_coscof_p1);
  y = simde_mm256_mul_ps(y, z);
  y = simde_mm256_add_ps(y, *(simde__m256*)m256_ps_coscof_p2);
  y = simde_mm256_mul_ps(y, z);
  y = simde_mm256_mul_ps(y, z);
  simde__m256 tmp = simde_mm256_mul_ps(z, *(simde__m256*)m256_ps_0p5);
  y = simde_mm256_sub_ps(y, tmp);
  y = simde_mm256_add_ps(y, *(simde__m256*)m256_ps_1);
  
  /* Evaluate the second polynom  (Pi/4 <= x <= 0) */

  simde__m256 y2 = *(simde__m256*)m256_ps_sincof_p0;
  y2 = simde_mm256_mul_ps(y2, z);
  y2 = simde_mm256_add_ps(y2, *(simde__m256*)m256_ps_sincof_p1);
  y2 = simde_mm256_mul_ps(y2, z);
  y2 = simde_mm256_add_ps(y2, *(simde__m256*)m256_ps_sincof_p2);
  y2 = simde_mm256_mul_ps(y2, z);
  y2 = simde_mm256_mul_ps(y2, x);
  y2 = simde_mm256_add_ps(y2, x);

  /* select the correct result from the two polynoms */  
  xmm3 = poly_mask;
  y2 = simde_mm256_and_ps(xmm3, y2); //, xmm3);
  y = simde_mm256_andnot_ps(xmm3, y);
  y = simde_mm256_add_ps(y,y2);
  /* update the sign */
  y = simde_mm256_xor_ps(y, sign_bit);

#ifdef __AVX__
  _mm256_zeroupper();
#endif
  return y;
}

simde__m256 mm256_cos_ps(simde__m256 x) {
  simde__m256 xmm1, xmm2 = simde_mm256_setzero_ps(), xmm3, y;
  simde__m256i emm0, emm2;
  /* take the absolute value */
  x = simde_mm256_and_ps(x, *(simde__m256*)m256_ps_inv_sign_mask);
  
  /* scale by 4/Pi */
  y = simde_mm256_mul_ps(x, *(simde__m256*)m256_ps_cephes_FOPI);

  /* store the integer part of y in mm0 */
  emm2 = simde_mm256_cvttps_epi32(y);
  /* j=(j+1) & (~1) (see the cephes sources) */
  emm2 = simde_mm256_add_epi32(emm2, *(simde__m256i*)m256_pi32_1);
  emm2 = simde_mm256_and_si256(emm2, *(simde__m256i*)m256_pi32_inv1);
  y = simde_mm256_cvtepi32_ps(emm2);

  emm2 = simde_mm256_sub_epi32(emm2, *(simde__m256i*)m256_pi32_2);
  
  /* get the swap sign flag */
  emm0 = simde_mm256_andnot_si256(emm2, *(simde__m256i*)m256_pi32_4);
  emm0 = simde_mm256_slli_epi32(emm0, 29);
  /* get the polynom selection mask */
  emm2 = simde_mm256_and_si256(emm2, *(simde__m256i*)m256_pi32_2);
  emm2 = simde_mm256_cmpeq_epi32(emm2, simde_mm256_setzero_si256());
  
  simde__m256 sign_bit = simde_mm256_castsi256_ps(emm0);
  simde__m256 poly_mask = simde_mm256_castsi256_ps(emm2);

  /* The magic pass: "Extended precision modular arithmetic" 
     x = ((x - y * DP1) - y * DP2) - y * DP3; */
  xmm1 = *(simde__m256*)m256_ps_minus_cephes_DP1;
  xmm2 = *(simde__m256*)m256_ps_minus_cephes_DP2;
  xmm3 = *(simde__m256*)m256_ps_minus_cephes_DP3;
  xmm1 = simde_mm256_mul_ps(y, xmm1);
  xmm2 = simde_mm256_mul_ps(y, xmm2);
  xmm3 = simde_mm256_mul_ps(y, xmm3);
  x = simde_mm256_add_ps(x, xmm1);
  x = simde_mm256_add_ps(x, xmm2);
  x = simde_mm256_add_ps(x, xmm3);
  
  /* Evaluate the first polynom  (0 <= x <= Pi/4) */
  y = *(simde__m256*)m256_ps_coscof_p0;
  simde__m256 z = simde_mm256_mul_ps(x,x);

  y = simde_mm256_mul_ps(y, z);
  y = simde_mm256_add_ps(y, *(simde__m256*)m256_ps_coscof_p1);
  y = simde_mm256_mul_ps(y, z);
  y = simde_mm256_add_ps(y, *(simde__m256*)m256_ps_coscof_p2);
  y = simde_mm256_mul_ps(y, z);
  y = simde_mm256_mul_ps(y, z);
  simde__m256 tmp = simde_mm256_mul_ps(z, *(simde__m256*)m256_ps_0p5);
  y = simde_mm256_sub_ps(y, tmp);
  y = simde_mm256_add_ps(y, *(simde__m256*)m256_ps_1);
  
  /* Evaluate the second polynom  (Pi/4 <= x <= 0) */

  simde__m256 y2 = *(simde__m256*)m256_ps_sincof_p0;
  y2 = simde_mm256_mul_ps(y2, z);
  y2 = simde_mm256_add_ps(y2, *(simde__m256*)m256_ps_sincof_p1);
  y2 = simde_mm256_mul_ps(y2, z);
  y2 = simde_mm256_add_ps(y2, *(simde__m256*)m256_ps_sincof_p2);
  y2 = simde_mm256_mul_ps(y2, z);
  y2 = simde_mm256_mul_ps(y2, x);
  y2 = simde_mm256_add_ps(y2, x);

  /* select the correct result from the two polynoms */  
  xmm3 = poly_mask;
  y2 = simde_mm256_and_ps(xmm3, y2); //, xmm3);
  y = simde_mm256_andnot_ps(xmm3, y);
  y = simde_mm256_add_ps(y,y2);
  /* update the sign */
  y = simde_mm256_xor_ps(y, sign_bit);

#ifdef __AVX__
  _mm256_zeroupper();
#endif
  return y;
}

