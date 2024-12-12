#pragma once

#include "Arm.h"

#if USE_SINGLE

#define RV_VECTOR float32x4_t

#define RV_LOAD vld1q_f32
#define RV_STORE vst1q_f32

static FORCEINLINE RV_VECTOR RV_ZERO() { return vdupq_n_f32(0); }

#define RV_SCALAR(x) vdupq_n_f32((Real)(x))
#define RV_NEGATE vnegq_f32
#define RV_ADD vaddq_f32
#define RV_SUB vsubq_f32
#define RV_MUL vmulq_f32
#define RV_MLA__ vmlaq_f32
#define RV_MLS__ vmlsq_f32

#define RV_SWAP_LH vrev64q_f32

static FORCEINLINE RV_VECTOR RV_DUPL(RV_VECTOR x) { return vtrn1q_f32(x, x); }

static FORCEINLINE RV_VECTOR RV_DUPH(RV_VECTOR x) { return vtrn2q_f32(x, x); }

static FORCEINLINE void RV_EVENODD2(RV_VECTOR *x0, RV_VECTOR *x1) {
  RV_VECTOR t = vcombine_f32(vget_low_f32(*x0), vget_low_f32(*x1));
  *x1 = vcombine_f32(vget_high_f32(*x0), vget_high_f32(*x1));
  *x0 = t;
}

static FORCEINLINE RV_VECTOR RV_LOAD_S(const Real *p, Long s) {
  return (RV_VECTOR){p[0], p[s], p[s * 2], p[s * 3]};
}

static FORCEINLINE void RV_STORE_S(Real *p, Long s, RV_VECTOR x) {
  vst1q_lane_f32(p, x, 0);
  vst1q_lane_f32(p + s, x, 1);
  vst1q_lane_f32(p + s * 2, x, 2);
  vst1q_lane_f32(p + s * 3, x, 3);
}

#include "Real.h"

#endif
