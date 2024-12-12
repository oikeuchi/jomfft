#pragma once

#include "Real_8.h"

#define CV_VECTOR RV_VECTOR

#if USE_SINGLE
static FORCEINLINE CV_VECTOR CV_LOAD_S__(const Complex *p0, const Complex *p1,
                                         const Complex *p2, const Complex *p3) {
  return _mm256_set_m128(
      _mm_castsi128_ps(_mm_set_epi64x(*(int64_t *)p3, *(int64_t *)p2)),
      _mm_castsi128_ps(_mm_set_epi64x(*(int64_t *)p1, *(int64_t *)p0)));
}

static FORCEINLINE void CV_STORE_S__(Complex *p_l, Complex *p_h, Long s,
                                     CV_VECTOR x) {
  {
    __m128 x_l = _mm256_castps256_ps128(x);
    _mm_storel_pi((__m64 *)p_l, x_l);
    _mm_storeh_pi((__m64 *)(p_l + s), x_l);
  }
  {
    __m128 x_h = _mm256_extractf128_ps(x, 1);
    _mm_storel_pi((__m64 *)(p_h), x_h);
    _mm_storeh_pi((__m64 *)(p_h + s), x_h);
  }
}

static FORCEINLINE CV_VECTOR CV_LOAD_S(const Complex *p, Long s) {
  return CV_LOAD_S__(p, p + s, p + s * 2, p + s * 3);
}

static FORCEINLINE void CV_STORE_S(Complex *p, Long s, CV_VECTOR x) {
  CV_STORE_S__(p, p + s * 2, s, x);
}

static FORCEINLINE CV_VECTOR CV_CONJ(CV_VECTOR x) {
  alignas(32) static const int32_t flags[8] = {
      0, X64_INT32_SIGN_FLAG, 0, X64_INT32_SIGN_FLAG,
      0, X64_INT32_SIGN_FLAG, 0, X64_INT32_SIGN_FLAG};
  return _mm256_xor_ps(x, _mm256_loadu_ps((const Real *)&flags[0]));
}

static FORCEINLINE CV_VECTOR CV_REVERSE(CV_VECTOR x) {
  x = _mm256_permute_ps(x, 0x4E);
  return _mm256_permute2f128_ps(x, x, 0x01);
}

static FORCEINLINE void RV_LOAD_RI_S(const Complex *p, Long s, RV_VECTOR *x_r,
                                     RV_VECTOR *x_i) {
  RV_VECTOR t0 = CV_LOAD_S__(p, p + s, p + s * 4, p + s * 5);
  RV_VECTOR t1 = CV_LOAD_S__(p + s * 2, p + s * 3, p + s * 6, p + s * 7);
  *x_r = _mm256_shuffle_ps(t0, t1, 0x88);
  *x_i = _mm256_shuffle_ps(t0, t1, 0xDD);
}

static FORCEINLINE void RV_STORE_RI_S(Complex *p, Long s, RV_VECTOR x_r,
                                      RV_VECTOR x_i) {
  RV_VECTOR t0 = _mm256_unpacklo_ps(x_r, x_i);
  RV_VECTOR t1 = _mm256_unpackhi_ps(x_r, x_i);
  CV_STORE_S__(p, p + s * 4, s, t0);
  CV_STORE_S__(p + s * 2, p + s * 6, s, t1);
}

#else
static FORCEINLINE CV_VECTOR CV_LOAD_S(const Complex *p, Long s) {
  return _mm512_insertf64x4(
      _mm512_castpd256_pd512(_mm256_set_m128d(
          _mm_loadu_pd((const Real *)(p + s)), _mm_loadu_pd((const Real *)p))),
      _mm256_set_m128d(_mm_loadu_pd((const Real *)(p + s * 3)),
                       _mm_loadu_pd((const Real *)(p + s * 2))),
      1);
}

static FORCEINLINE void CV_STORE_S(Complex *p, Long s, CV_VECTOR x) {
  _mm_storeu_pd((Real *)p, _mm512_castpd512_pd128(x));
  _mm_storeu_pd((Real *)(p + s), _mm512_extractf64x2_pd((x), 1));
  _mm_storeu_pd((Real *)(p + s * 2), _mm512_extractf64x2_pd((x), 2));
  _mm_storeu_pd((Real *)(p + s * 3), _mm512_extractf64x2_pd((x), 3));
}

static FORCEINLINE CV_VECTOR CV_CONJ(CV_VECTOR x) {
  alignas(64) static const int64_t flags[8] = {
      0, X64_INT64_SIGN_FLAG, 0, X64_INT64_SIGN_FLAG,
      0, X64_INT64_SIGN_FLAG, 0, X64_INT64_SIGN_FLAG};
  return _mm512_xor_pd(x, _mm512_castsi512_pd(_mm512_loadu_epi64(flags)));
}

static FORCEINLINE CV_VECTOR CV_REVERSE(CV_VECTOR x) {
  return _mm512_shuffle_f64x2(x, x, 0x1B);
}
#endif

#define CV_TRANSPOSE_2 RV_EVENODD2

static FORCEINLINE void CV_TRANSPOSE_4(CV_VECTOR *x0, CV_VECTOR *x1,
                                       CV_VECTOR *x2, CV_VECTOR *x3) {
  RV_EVENODD2(x0, x1);
  RV_EVENODD2(x2, x3);
  RV_EVENODD4(x0, x2);
  RV_EVENODD4(x1, x3);
}

#include "Complex.h"
