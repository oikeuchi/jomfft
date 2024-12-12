#pragma once

#include "X64.h"

#if USE_SINGLE

#define RV_VECTOR __m256

#define RV_LOAD _mm256_loadu_ps
#define RV_STORE _mm256_storeu_ps

#define RV_ZERO _mm256_setzero_ps
#define RV_NEGATE(x) _mm256_sub_ps(_mm256_setzero_ps(), (x))
#define RV_SCALAR(x) _mm256_set1_ps((Real)(x))
#define RV_ADD _mm256_add_ps
#define RV_SUB _mm256_sub_ps
#define RV_ADDSUB _mm256_addsub_ps
#define RV_MUL _mm256_mul_ps
#define RV_FMADD _mm256_fmadd_ps
#define RV_FMSUB _mm256_fmsub_ps
#define RV_FNMADD _mm256_fnmadd_ps
#define RV_FMADDSUB _mm256_fmaddsub_ps
#define RV_FMSUBADD _mm256_fmsubadd_ps

#define RV_SWAP_LH(x) _mm256_permute_ps((x), 0xB1)
#define RV_DUPL _mm256_moveldup_ps
#define RV_DUPH _mm256_movehdup_ps

static FORCEINLINE void RV_EVENODD2(RV_VECTOR *x0, RV_VECTOR *x1) {
  RV_VECTOR t = _mm256_shuffle_ps(*x0, *x1, 0x44);
  *x1 = _mm256_shuffle_ps(*x0, *x1, 0xEE);
  *x0 = t;
}

static FORCEINLINE void RV_EVENODD4(RV_VECTOR *x0, RV_VECTOR *x1) {
  RV_VECTOR t = _mm256_permute2f128_ps(*x0, *x1, 0x21);
  *x0 = _mm256_blend_ps(*x0, t, 0xF0);
  *x1 = _mm256_blend_ps(t, *x1, 0xF0);
}

static FORCEINLINE RV_VECTOR RV_LOAD_S(const Real *p, Long s) {
  return _mm256_set_ps(p[s * 7], p[s * 6], p[s * 5], p[s * 4], p[s * 3],
                       p[s * 2], p[s], p[0]);
}

static FORCEINLINE void RV_STORE_S(Real *p, Long s, RV_VECTOR x) {
  int *v = (int *)p;
  {
    __m128 x_l = _mm256_castps256_ps128(x);
    v[0] = _mm_extract_ps(x_l, 0);
    v[s] = _mm_extract_ps(x_l, 1);
    v[s * 2] = _mm_extract_ps(x_l, 2);
    v[s * 3] = _mm_extract_ps(x_l, 3);
  }
  {
    __m128 x_h = _mm256_extractf128_ps(x, 1);
    v[s * 4] = _mm_extract_ps(x_h, 0);
    v[s * 5] = _mm_extract_ps(x_h, 1);
    v[s * 6] = _mm_extract_ps(x_h, 2);
    v[s * 7] = _mm_extract_ps(x_h, 3);
  }
}

#else

#define RV_VECTOR __m512d

#define RV_LOAD _mm512_loadu_pd
#define RV_STORE _mm512_storeu_pd

#define RV_ZERO _mm512_setzero_pd
#define RV_NEGATE(x) _mm512_sub_pd(_mm512_setzero_pd(), (x))
#define RV_SCALAR _mm512_set1_pd
#define RV_ADD _mm512_add_pd
#define RV_SUB _mm512_sub_pd
#define RV_MUL _mm512_mul_pd
#define RV_FMADD _mm512_fmadd_pd
#define RV_FMSUB _mm512_fmsub_pd
#define RV_FNMADD _mm512_fnmadd_pd
#define RV_FMADDSUB _mm512_fmaddsub_pd
#define RV_FMSUBADD _mm512_fmsubadd_pd
#define RV_FNMADDSUB _mm512_fnmaddsub_pd
#define RV_FNMSUBADD _mm512_fnmsubadd_pd

#define RV_SWAP_LH(x) _mm512_permute_pd((x), 0x55)
#define RV_DUPL _mm512_movedup_pd
#define RV_DUPH(x) _mm512_unpackhi_pd((x), (x))

static FORCEINLINE RV_VECTOR RV_ADDSUB(RV_VECTOR x, RV_VECTOR y) {
  return RV_FMADDSUB(RV_SCALAR(1), x, y);
}

static FORCEINLINE void RV_EVENODD2(RV_VECTOR *x0, RV_VECTOR *x1) {
  RV_VECTOR t = _mm512_permutex_pd(*x0, 0x4E);
  *x0 = _mm512_mask_blend_pd(0xCC, *x0, _mm512_permutex_pd(*x1, 0x4E));
  *x1 = _mm512_mask_blend_pd(0xCC, t, *x1);
}

static FORCEINLINE void RV_EVENODD4(RV_VECTOR *x0, RV_VECTOR *x1) {
  RV_VECTOR t = _mm512_shuffle_f64x2(*x0, *x1, 0x4E);
  *x0 = _mm512_mask_blend_pd(0xF0, *x0, t);
  *x1 = _mm512_mask_blend_pd(0xF0, t, *x1);
}

#endif

#include "Real.h"
