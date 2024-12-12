#pragma once

#include "Real_2.h"

#define CV_VECTOR RV_VECTOR

static FORCEINLINE CV_VECTOR CV_LOAD_S(const Complex *p, Long s) {
  (void)s;
  return RV_LOAD((const Real *)p);
}

static FORCEINLINE void CV_STORE_S(Complex *p, Long s, CV_VECTOR x) {
  (void)s;
  RV_STORE((Real *)p, x);
}

#if USE_SINGLE

static FORCEINLINE CV_VECTOR CV_CONJ(CV_VECTOR x) {
  alignas(16) static const int32_t flags[4] = {0, X64_INT32_SIGN_FLAG, 0,
                                               X64_INT32_SIGN_FLAG};
  return _mm_xor_ps(x, _mm_loadu_ps((const Real *)&flags[0]));
}

static FORCEINLINE CV_VECTOR CV_REVERSE(CV_VECTOR x) { return x; }

static FORCEINLINE void RV_LOAD_RI_S(const Complex *p, Long s, RV_VECTOR *x_r,
                                     RV_VECTOR *x_i) {
  RV_VECTOR t = _mm_unpacklo_ps(RV_LOAD((Real *)p), RV_LOAD((Real *)(p + s)));
  *x_r = t;
  *x_i = _mm_permute_ps(t, 0x4E);
}

static FORCEINLINE void RV_STORE_RI_S(Complex *p, Long s, RV_VECTOR x_r,
                                      RV_VECTOR x_i) {
  RV_VECTOR t = _mm_unpacklo_ps(x_r, x_i);
  RV_STORE((Real *)p, t);
  RV_STORE((Real *)(p + s), _mm_permute_ps(t, 0x4E));
}

#else

#define CV_VECTOR RV_VECTOR

static FORCEINLINE CV_VECTOR CV_CONJ(CV_VECTOR x) {
  alignas(16) static const int64_t flags[2] = {0, X64_INT64_SIGN_FLAG};
  return _mm_xor_pd(x, _mm_loadu_pd((const Real *)&flags[0]));
}

static FORCEINLINE CV_VECTOR CV_REVERSE(CV_VECTOR x) { return x; }

static FORCEINLINE void RV_LOAD_RI_S(const Complex *p, Long s, RV_VECTOR *x_r,
                                     RV_VECTOR *x_i) {
  RV_VECTOR t0 = RV_LOAD((const Real *)p);
  RV_VECTOR t1 = RV_LOAD((const Real *)(p + s));
  *x_r = _mm_unpacklo_pd(t0, t1);
  *x_i = _mm_unpackhi_pd(t0, t1);
}

static FORCEINLINE void RV_STORE_RI_S(Complex *p, Long s, RV_VECTOR x_r,
                                      RV_VECTOR x_i) {
  RV_VECTOR t0 = _mm_unpacklo_pd(x_r, x_i);
  RV_VECTOR t1 = _mm_unpackhi_pd(x_r, x_i);
  RV_STORE((Real *)p, t0);
  RV_STORE((Real *)(p + s), t1);
}
#endif

#include "Complex.h"
