#pragma once

#include "../math/Math.h"
#include "Simd.h"

#define RV_VECTOR Complex

static FORCEINLINE RV_VECTOR RV_LOAD(const Real *p) {
  return *((const Complex *)p);
}

static FORCEINLINE void RV_STORE(Real *p, RV_VECTOR x) { *((Complex *)p) = x; }

static FORCEINLINE RV_VECTOR RV_ZERO() { return Complex_(0, 0); }

static FORCEINLINE RV_VECTOR RV_NEGATE(RV_VECTOR x) {
  return Complex_(-C_REAL(x), -C_IMAG(x));
}

static FORCEINLINE RV_VECTOR RV_SCALAR(Real x) { return Complex_(x, x); }

static FORCEINLINE RV_VECTOR RV_ADD(RV_VECTOR x, RV_VECTOR y) {
  return C_ADD(x, y);
}

static FORCEINLINE RV_VECTOR RV_SUB(RV_VECTOR x, RV_VECTOR y) {
  return C_SUB(x, y);
}

static FORCEINLINE RV_VECTOR RV_MUL(RV_VECTOR x, RV_VECTOR y) {
  return Complex_(C_REAL(x) * C_REAL(y), C_IMAG(x) * C_IMAG(y));
}

static FORCEINLINE RV_VECTOR RV_FMADD(RV_VECTOR a, RV_VECTOR x, RV_VECTOR y) {
  return Complex_(C_REAL(a) * C_REAL(x) + C_REAL(y),
                  C_IMAG(a) * C_IMAG(x) + C_IMAG(y));
}

static FORCEINLINE RV_VECTOR RV_FMSUB(RV_VECTOR a, RV_VECTOR x, RV_VECTOR y) {
  return Complex_(C_REAL(a) * C_REAL(x) - C_REAL(y),
                  C_IMAG(a) * C_IMAG(x) - C_IMAG(y));
}

static FORCEINLINE RV_VECTOR RV_FNMADD(RV_VECTOR a, RV_VECTOR x, RV_VECTOR y) {
  return Complex_(-C_REAL(a) * C_REAL(x) + C_REAL(y),
                  -C_IMAG(a) * C_IMAG(x) + C_IMAG(y));
}

static FORCEINLINE RV_VECTOR RV_SWAP_LH(RV_VECTOR x) {
  return Complex_(C_IMAG(x), C_REAL(x));
}

static FORCEINLINE RV_VECTOR RV_LOAD_S(const Real *p, Long s) {
  return Complex_(*p, *(p + s));
}

static FORCEINLINE void RV_STORE_S(Real *p, Long s, RV_VECTOR x) {
  *p = C_REAL(x);
  *(p + s) = C_IMAG(x);
}

static FORCEINLINE void RV_LOAD_RI_S(const Complex *p, Long s, RV_VECTOR *x_r,
                                     RV_VECTOR *x_i) {
  const Complex z0 = *p;
  const Complex z1 = *(p + s);
  *x_r = Complex_(C_REAL(z0), C_REAL(z1));
  *x_i = Complex_(C_IMAG(z0), C_IMAG(z1));
}

static FORCEINLINE void RV_STORE_RI_S(Complex *p, Long s, RV_VECTOR x_r,
                                      RV_VECTOR x_i) {
  *p = Complex_(C_REAL(x_r), C_REAL(x_i));
  *(p + s) = Complex_(C_IMAG(x_r), C_IMAG(x_i));
}

#include "RealCommon.h"
