#pragma once

#include "X64.h"

#if USE_SINGLE

#define RV_VECTOR __m128

static FORCEINLINE RV_VECTOR RV_LOAD(const Real *p) {
  return _mm_loadl_pi(_mm_setzero_ps(), (const __m64 *)(p));
}

static FORCEINLINE void RV_STORE(Real *p, RV_VECTOR x) {
  _mm_storel_pi((__m64 *)(p), x);
}

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

static FORCEINLINE RV_VECTOR RV_LOAD_S(const Real *p, Long s) {
  alignas(16) Real t[] = {p[0], p[s]};
  return RV_LOAD(t);
}

static FORCEINLINE void RV_STORE_S(Real *p, Long s, RV_VECTOR x) {
  int *v = (int *)p;
  v[0] = _mm_extract_ps(x, 0);
  v[s] = _mm_extract_ps(x, 1);
}

#else

#define RV_VECTOR __m128d

#define RV_LOAD _mm_loadu_pd
#define RV_STORE _mm_storeu_pd

#define RV_ZERO _mm_setzero_pd
#define RV_NEGATE(x) _mm_sub_pd(_mm_setzero_pd(), (x))
#define RV_SCALAR _mm_set1_pd
#define RV_ADD _mm_add_pd
#define RV_SUB _mm_sub_pd
#define RV_ADDSUB _mm_addsub_pd
#define RV_MUL _mm_mul_pd
#define RV_FMADD _mm_fmadd_pd
#define RV_FMSUB _mm_fmsub_pd
#define RV_FNMADD _mm_fnmadd_pd
#define RV_FMADDSUB _mm_fmaddsub_pd
#define RV_FMSUBADD _mm_fmsubadd_pd

#define RV_SWAP_LH(x) _mm_permute_pd((x), 0x1)
#define RV_DUPL _mm_movedup_pd
#define RV_DUPH(x) _mm_unpackhi_pd((x), (x))

static FORCEINLINE RV_VECTOR RV_LOAD_S(const Real *p, Long s) {
  return _mm_set_pd(p[s], p[0]);
}

static FORCEINLINE void RV_STORE_S(Real *p, Long s, RV_VECTOR x) {
  __m128i t = _mm_castpd_si128(x);
  *((int64_t *)p) = _mm_extract_epi64(t, 0);
  *((int64_t *)(p + s)) = _mm_extract_epi64(t, 1);
}

#endif

#include "Real.h"
