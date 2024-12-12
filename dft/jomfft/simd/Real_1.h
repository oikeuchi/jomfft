#pragma once

#include "../math/Math.h"
#include "Simd.h"

#define RV_VECTOR Real

static FORCEINLINE RV_VECTOR RV_LOAD(const Real *p) { return *p; }

static FORCEINLINE void RV_STORE(Real *p, RV_VECTOR x) { *p = x; }

static FORCEINLINE RV_VECTOR RV_ZERO() { return 0; }

static FORCEINLINE RV_VECTOR RV_NEGATE(Real x) { return -x; }

static FORCEINLINE RV_VECTOR RV_SCALAR(Real x) { return x; }

static FORCEINLINE RV_VECTOR RV_ADD(Real x, Real y) { return x + y; }

static FORCEINLINE RV_VECTOR RV_SUB(Real x, Real y) { return x - y; }

static FORCEINLINE RV_VECTOR RV_MUL(Real x, Real y) { return x * y; }

static FORCEINLINE RV_VECTOR RV_FMADD(Real a, Real x, Real y) {
  return a * x + y;
}

static FORCEINLINE RV_VECTOR RV_FMSUB(Real a, Real x, Real y) {
  return a * x - y;
}

static FORCEINLINE RV_VECTOR RV_FNMADD(Real a, Real x, Real y) {
  return -a * x + y;
}

static FORCEINLINE RV_VECTOR RV_LOAD_S(const Real *p, Long s) {
  (void)s;
  return RV_LOAD(p);
}

static FORCEINLINE void RV_STORE_S(Real *p, Long s, RV_VECTOR x) {
  (void)s;
  RV_STORE(p, x);
}

static FORCEINLINE void RV_LOAD_RI_S(const Complex *p, Long s, RV_VECTOR *x_r,
                                     RV_VECTOR *x_i) {
  (void)s;
  *x_r = C_REAL(*p);
  *x_i = C_IMAG(*p);
}

static FORCEINLINE void RV_STORE_RI_S(Complex *p, Long s, RV_VECTOR x_r,
                                      RV_VECTOR x_i) {
  (void)s;
  *p = Complex_(x_r, x_i);
}

#include "RealCommon.h"
