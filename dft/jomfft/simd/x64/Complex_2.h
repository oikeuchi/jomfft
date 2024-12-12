#pragma once

#include "Real_4.h"

#define CV_VECTOR RV_VECTOR

#if USE_SINGLE
static FORCEINLINE CV_VECTOR CV_LOAD_S(const Complex *p, Long s) {
  return _mm_castsi128_ps(_mm_set_epi64x(*(int64_t *)(p + s), *(int64_t *)p));
}

static FORCEINLINE void CV_STORE_S(Complex *p, Long s, CV_VECTOR x) {
  _mm_storel_pi((__m64 *)p, x);
  _mm_storeh_pi((__m64 *)(p + s), x);
}

static FORCEINLINE CV_VECTOR CV_CONJ(CV_VECTOR x) {
  alignas(16) static const int32_t flags[4] = {0, X64_INT32_SIGN_FLAG, 0,
                                               X64_INT32_SIGN_FLAG};
  return _mm_xor_ps(x, _mm_loadu_ps((const Real *)&flags[0]));
}

static FORCEINLINE CV_VECTOR CV_REVERSE(CV_VECTOR x) {
  return _mm_permute_ps(x, 0x4E);
}

static FORCEINLINE void RV_LOAD_RI_S(const Complex *p, Long s, RV_VECTOR *x_r,
                                     RV_VECTOR *x_i) {
  RV_VECTOR t0 = CV_LOAD_S(p, s);
  RV_VECTOR t1 = CV_LOAD_S(p + s * 2, s);
  *x_r = _mm_shuffle_ps(t0, t1, 0x88);
  *x_i = _mm_shuffle_ps(t0, t1, 0xDD);
}

static FORCEINLINE void RV_STORE_RI_S(Complex *p, Long s, RV_VECTOR x_r,
                                      RV_VECTOR x_i) {
  RV_VECTOR t0 = _mm_unpacklo_ps(x_r, x_i);
  RV_VECTOR t1 = _mm_unpackhi_ps(x_r, x_i);
  CV_STORE_S(p, s, t0);
  CV_STORE_S(p + s * 2, s, t1);
}

#else
static FORCEINLINE CV_VECTOR CV_LOAD_S(const Complex *p, Long s) {
  return _mm256_set_m128d(_mm_loadu_pd((const Real *)(p + s)),
                          _mm_loadu_pd((const Real *)p));
}

static FORCEINLINE void CV_STORE_S(Complex *p, Long s, CV_VECTOR x) {
  _mm_storeu_pd((Real *)p, _mm256_castpd256_pd128(x));
  _mm_storeu_pd((Real *)(p + s), _mm256_extractf128_pd(x, 1));
}

static FORCEINLINE CV_VECTOR CV_CONJ(CV_VECTOR x) {
  alignas(32) static const int64_t flags[4] = {0, X64_INT64_SIGN_FLAG, 0,
                                               X64_INT64_SIGN_FLAG};
  return _mm256_xor_pd(x, _mm256_loadu_pd((const Real *)&flags[0]));
}

static FORCEINLINE CV_VECTOR CV_REVERSE(CV_VECTOR x) {
  return _mm256_permute2f128_pd(x, x, 0x21);
}

static FORCEINLINE void RV_LOAD_RI_S(const Complex *p, Long s, RV_VECTOR *x_r,
                                     RV_VECTOR *x_i) {
  RV_VECTOR t0 = CV_LOAD_S(p, s * 2);
  RV_VECTOR t1 = CV_LOAD_S(p + s, s * 2);
  *x_r = _mm256_unpacklo_pd(t0, t1);
  *x_i = _mm256_unpackhi_pd(t0, t1);
}

static FORCEINLINE void RV_STORE_RI_S(Complex *p, Long s, RV_VECTOR x_r,
                                      RV_VECTOR x_i) {
  RV_VECTOR t0 = _mm256_unpacklo_pd(x_r, x_i);
  RV_VECTOR t1 = _mm256_unpackhi_pd(x_r, x_i);
  CV_STORE_S(p, s * 2, t0);
  CV_STORE_S(p + s, s * 2, t1);
}
#endif

#define CV_TRANSPOSE_2 RV_EVENODD2

#include "Complex.h"
