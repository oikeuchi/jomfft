#pragma once

#include "X64.h"

#if USE_SINGLE

#define RV_VECTOR __m128

#define RV_LOAD _mm_loadu_ps
#define RV_STORE _mm_storeu_ps

#define RV_ZERO _mm_setzero_ps
#define RV_NEGATE(x) _mm_sub_ps(_mm_setzero_ps(), (x))
#define RV_SCALAR(x) _mm_set1_ps((Real)(x))
#define RV_ADD _mm_add_ps
#define RV_SUB _mm_sub_ps
#define RV_ADDSUB _mm_addsub_ps
#define RV_MUL _mm_mul_ps
#define RV_FMADD _mm_fmadd_ps
#define RV_FMSUB _mm_fmsub_ps
#define RV_FNMADD _mm_fnmadd_ps
#define RV_FMADDSUB _mm_fmaddsub_ps
#define RV_FMSUBADD _mm_fmsubadd_ps

#define RV_SWAP_LH(x) _mm_permute_ps((x), 0xB1)
#define RV_DUPL _mm_moveldup_ps
#define RV_DUPH _mm_movehdup_ps

static FORCEINLINE void RV_EVENODD2(RV_VECTOR *x0, RV_VECTOR *x1) {
  RV_VECTOR t = _mm_shuffle_ps(*x0, *x1, 0x44);
  *x1 = _mm_shuffle_ps(*x0, *x1, 0xEE);
  *x0 = t;
}

static FORCEINLINE RV_VECTOR RV_LOAD_S(const Real *p, Long s) {
  return _mm_set_ps(p[s * 3], p[s * 2], p[s], p[0]);
}

static FORCEINLINE void RV_STORE_S(Real *p, Long s, RV_VECTOR x) {
  int *v = (int *)p;
  v[0] = _mm_extract_ps(x, 0);
  v[s] = _mm_extract_ps(x, 1);
  v[s * 2] = _mm_extract_ps(x, 2);
  v[s * 3] = _mm_extract_ps(x, 3);
}

#else

#define RV_VECTOR __m256d

#define RV_LOAD _mm256_loadu_pd
#define RV_STORE _mm256_storeu_pd

#define RV_ZERO _mm256_setzero_pd
#define RV_NEGATE(x) _mm256_sub_pd(_mm256_setzero_pd(), (x))
#define RV_SCALAR _mm256_set1_pd
#define RV_ADD _mm256_add_pd
#define RV_SUB _mm256_sub_pd
#define RV_ADDSUB _mm256_addsub_pd
#define RV_MUL _mm256_mul_pd
#define RV_FMADD _mm256_fmadd_pd
#define RV_FMSUB _mm256_fmsub_pd
#define RV_FNMADD _mm256_fnmadd_pd
#define RV_FMADDSUB _mm256_fmaddsub_pd
#define RV_FMSUBADD _mm256_fmsubadd_pd

#define RV_SWAP_LH(x) _mm256_permute_pd((x), 0x5)
#define RV_DUPL _mm256_movedup_pd
#define RV_DUPH(x) _mm256_unpackhi_pd((x), (x))

static FORCEINLINE void RV_EVENODD2(RV_VECTOR *x0, RV_VECTOR *x1) {
  RV_VECTOR t = _mm256_permute2f128_pd(*x0, *x1, 0x21);
  *x0 = _mm256_blend_pd(*x0, t, 0xC);
  *x1 = _mm256_blend_pd(t, *x1, 0xC);
}

static FORCEINLINE RV_VECTOR RV_LOAD_S(const Real *p, Long s) {
  return _mm256_set_pd(p[s * 3], p[s * 2], p[s], p[0]);
}

static FORCEINLINE void RV_STORE_S(Real *p, Long s, RV_VECTOR x) {
  {
    __m128i x_l = _mm_castpd_si128(_mm256_castpd256_pd128(x));
    *((int64_t *)p) = _mm_extract_epi64(x_l, 0);
    *((int64_t *)(p + s)) = _mm_extract_epi64(x_l, 1);
  }
  {
    __m128i x_h = _mm_castpd_si128(_mm256_extractf128_pd(x, 1));
    *((int64_t *)(p + s * 2)) = _mm_extract_epi64(x_h, 0);
    *((int64_t *)(p + s * 3)) = _mm_extract_epi64(x_h, 1);
  }
}

#endif

#include "Real.h"
