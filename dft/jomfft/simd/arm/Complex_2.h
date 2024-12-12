#pragma once

#include "Real_4.h"

#define CV_VECTOR RV_VECTOR

#if USE_SINGLE

static FORCEINLINE CV_VECTOR CV_LOAD_S(const Complex *p, Long s) {
  return vcombine_f32(vld1_f32((Real *)p), vld1_f32((Real *)(p + s)));
}

static FORCEINLINE void CV_STORE_S(Complex *p, Long s, CV_VECTOR x) {
  vst1_f32((Real *)p, vget_low_f32(x));
  vst1_f32((Real *)(p + s), vget_high_f32(x));
}

static FORCEINLINE CV_VECTOR CV_CONJ(CV_VECTOR x) {
  static const uint32x4_t flags = {0, ARM_UINT32_SIGN_FLAG, 0,
                                   ARM_UINT32_SIGN_FLAG};
  return vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(x), flags));
}

static FORCEINLINE CV_VECTOR CV_REVERSE(CV_VECTOR x) {
  return vcombine_f32(vget_high_f32(x), vget_low_f32(x));
}

static FORCEINLINE void RV_LOAD_RI_S(const Complex *p, Long s, RV_VECTOR *x_r,
                                     RV_VECTOR *x_i) {
  RV_VECTOR t0 = CV_LOAD_S(p, s);
  RV_VECTOR t1 = CV_LOAD_S(p + s * 2, s);
  *x_r = vuzp1q_f32(t0, t1);
  *x_i = vuzp2q_f32(t0, t1);
}

static FORCEINLINE void RV_STORE_RI_S(Complex *p, Long s, RV_VECTOR x_r,
                                      RV_VECTOR x_i) {
  RV_VECTOR t0 = vzip1q_f32(x_r, x_i);
  RV_VECTOR t1 = vzip2q_f32(x_r, x_i);
  CV_STORE_S(p, s, t0);
  CV_STORE_S(p + s * 2, s, t1);
}
#endif

#define CV_TRANSPOSE_2 RV_EVENODD2

#include "Complex.h"
