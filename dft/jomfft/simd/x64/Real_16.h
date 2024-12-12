#pragma once

#include "X64.h"

#if USE_SINGLE

#define RV_VECTOR __m512

#define RV_LOAD _mm512_loadu_ps
#define RV_STORE _mm512_storeu_ps

#define RV_ZERO _mm512_setzero_ps
#define RV_NEGATE(x) _mm512_sub_ps(_mm512_setzero_ps(), (x))
#define RV_SCALAR(x) _mm512_set1_ps((Real)(x))
#define RV_ADD _mm512_add_ps
#define RV_SUB _mm512_sub_ps
#define RV_ADDSUB _mm512_addsub_ps
#define RV_MUL _mm512_mul_ps
#define RV_FMADD _mm512_fmadd_ps
#define RV_FMSUB _mm512_fmsub_ps
#define RV_FNMADD _mm512_fnmadd_ps
#define RV_FMADDSUB _mm512_fmaddsub_ps
#define RV_FMSUBADD _mm512_fmsubadd_ps

#define RV_SWAP_LH(x) _mm512_permute_ps((x), 0xB1)
#define RV_DUPL _mm512_moveldup_ps
#define RV_DUPH _mm512_movehdup_ps

static FORCEINLINE RV_VECTOR RV_ADDSUB(RV_VECTOR x, RV_VECTOR y) {
  return RV_FMADDSUB(RV_SCALAR(1), x, y);
}

static FORCEINLINE void RV_EVENODD2(RV_VECTOR *x0, RV_VECTOR *x1) {
  RV_VECTOR t = _mm512_shuffle_ps(*x0, *x1, 0x44);
  *x1 = _mm512_shuffle_ps(*x0, *x1, 0xEE);
  *x0 = t;
}

static FORCEINLINE void RV_EVENODD4(RV_VECTOR *x0, RV_VECTOR *x1) {
  RV_VECTOR t = _mm512_shuffle_f32x4(*x0, *x0, 0xB1);
  *x0 = _mm512_mask_blend_ps(0xF0F0, *x0, _mm512_shuffle_f32x4(*x1, *x1, 0xB1));
  *x1 = _mm512_mask_blend_ps(0xF0F0, t, *x1);
}

static FORCEINLINE void RV_EVENODD8(RV_VECTOR *x0, RV_VECTOR *x1) {
  RV_VECTOR t = _mm512_shuffle_f32x4(*x0, *x1, 0x4E);
  *x0 = _mm512_mask_blend_ps(0xFF00, *x0, t);
  *x1 = _mm512_mask_blend_ps(0xFF00, t, *x1);
}

#endif

#include "Real.h"
