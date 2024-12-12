#pragma once

#include "Real_16.h"

#define CV_VECTOR RV_VECTOR

#if USE_SINGLE
static FORCEINLINE CV_VECTOR CV_LOAD_S(const Complex *p, Long s) {
  return _mm512_insertf32x8(
      _mm512_castps256_ps512(_mm256_set_m128(
          _mm_castsi128_ps(
              _mm_set_epi64x(*(int64_t *)(p + s * 3), *(int64_t *)(p + s * 2))),
          _mm_castsi128_ps(
              _mm_set_epi64x(*(int64_t *)(p + s), *(int64_t *)p)))),
      _mm256_set_m128(_mm_castsi128_ps(_mm_set_epi64x(*(int64_t *)(p + s * 7),
                                                      *(int64_t *)(p + s * 6))),
                      _mm_castsi128_ps(_mm_set_epi64x(
                          *(int64_t *)(p + s * 5), *(int64_t *)(p + s * 4)))),
      1);
}

static FORCEINLINE void CV_STORE_S(Complex *p, Long s, CV_VECTOR x) {
  {
    __m128 x_0 = _mm512_castps512_ps128(x);
    _mm_storel_pi((__m64 *)p, x_0);
    _mm_storeh_pi((__m64 *)(p + s), x_0);
  }
  {
    __m128 x_1 = _mm512_extractf32x4_ps((x), 1);
    _mm_storel_pi((__m64 *)(p + s * 2), x_1);
    _mm_storeh_pi((__m64 *)(p + s * 3), x_1);
  }
  {
    __m128 x_2 = _mm512_extractf32x4_ps((x), 2);
    _mm_storel_pi((__m64 *)(p + s * 4), x_2);
    _mm_storeh_pi((__m64 *)(p + s * 5), x_2);
  }
  {
    __m128 x_3 = _mm512_extractf32x4_ps((x), 3);
    _mm_storel_pi((__m64 *)(p + s * 6), x_3);
    _mm_storeh_pi((__m64 *)(p + s * 7), x_3);
  }
}

static FORCEINLINE CV_VECTOR CV_CONJ(CV_VECTOR x) {
  alignas(64) static const int32_t flags[16] = {
      0, X64_INT32_SIGN_FLAG, 0, X64_INT32_SIGN_FLAG, 0, X64_INT32_SIGN_FLAG,
      0, X64_INT32_SIGN_FLAG, 0, X64_INT32_SIGN_FLAG, 0, X64_INT32_SIGN_FLAG,
      0, X64_INT32_SIGN_FLAG, 0, X64_INT32_SIGN_FLAG};
  return _mm512_xor_ps(x, _mm512_castsi512_ps(_mm512_loadu_epi32(flags)));
}

static FORCEINLINE CV_VECTOR CV_REVERSE(CV_VECTOR x) {
  x = _mm512_permute_ps(x, 0x4E);
  return _mm512_shuffle_f32x4(x, x, 0x1B);
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

static FORCEINLINE void CV_TRANSPOSE_8(CV_VECTOR *x0, CV_VECTOR *x1,
                                       CV_VECTOR *x2, CV_VECTOR *x3,
                                       CV_VECTOR *x4, CV_VECTOR *x5,
                                       CV_VECTOR *x6, CV_VECTOR *x7) {
  RV_EVENODD2(x0, x1);
  RV_EVENODD2(x2, x3);
  RV_EVENODD2(x4, x5);
  RV_EVENODD2(x6, x7);
  RV_EVENODD4(x0, x2);
  RV_EVENODD4(x1, x3);
  RV_EVENODD4(x4, x6);
  RV_EVENODD4(x5, x7);
  RV_EVENODD8(x0, x4);
  RV_EVENODD8(x1, x5);
  RV_EVENODD8(x2, x6);
  RV_EVENODD8(x3, x7);
}

#include "Complex.h"
