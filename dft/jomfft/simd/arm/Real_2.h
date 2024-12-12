#pragma once

#include "Arm.h"

#if USE_SINGLE

#define RV_VECTOR float32x2_t

static FORCEINLINE RV_VECTOR RV_LOAD(const Real *p) { return vld1_f32(p); }

static FORCEINLINE void RV_STORE(Real *p, RV_VECTOR x) { vst1_f32(p, x); }

static FORCEINLINE RV_VECTOR RV_ZERO() { return vdup_n_f32(0); }

#define RV_SCALAR(x) vdup_n_f32((Real)(x))
#define RV_NEGATE vneg_f32
#define RV_ADD vadd_f32
#define RV_SUB vsub_f32
#define RV_MUL vmul_f32
#define RV_MLA__ vmla_f32
#define RV_MLS__ vmls_f32

#define RV_SWAP_LH vrev64_f32

static FORCEINLINE RV_VECTOR RV_DUPL(RV_VECTOR x) { return vtrn1_f32(x, x); }

static FORCEINLINE RV_VECTOR RV_DUPH(RV_VECTOR x) { return vtrn2_f32(x, x); }

static FORCEINLINE RV_VECTOR RV_LOAD_S(const Real *p, Long s) {
  return (RV_VECTOR){p[0], p[s]};
}

static FORCEINLINE void RV_STORE_S(Real *p, Long s, RV_VECTOR x) {
  vst1_lane_f32(p, x, 0);
  vst1_lane_f32(p + s, x, 1);
}

#else

#define RV_VECTOR float64x2_t

#define RV_LOAD vld1q_f64
#define RV_STORE vst1q_f64

static FORCEINLINE RV_VECTOR RV_ZERO() { return vdupq_n_f64(0); }

#define RV_SCALAR vdupq_n_f64
#define RV_NEGATE vnegq_f64
#define RV_ADD vaddq_f64
#define RV_SUB vsubq_f64
#define RV_MUL vmulq_f64
#define RV_MLA__ vmlaq_f64
#define RV_MLS__ vmlsq_f64

static FORCEINLINE RV_VECTOR RV_SWAP_LH(RV_VECTOR x) {
  return vcombine_f64(vget_high_f64(x), vget_low_f64(x));
}

static FORCEINLINE RV_VECTOR RV_DUPL(RV_VECTOR x) { return vtrn1q_f64(x, x); }

static FORCEINLINE RV_VECTOR RV_DUPH(RV_VECTOR x) { return vtrn2q_f64(x, x); }

static FORCEINLINE RV_VECTOR RV_LOAD_S(const Real *p, Long s) {
  return (RV_VECTOR){p[0], p[s]};
}

static FORCEINLINE void RV_STORE_S(Real *p, Long s, RV_VECTOR x) {
  vst1q_lane_f64(p, x, 0);
  vst1q_lane_f64(p + s, x, 1);
}

#endif

#include "Real.h"
