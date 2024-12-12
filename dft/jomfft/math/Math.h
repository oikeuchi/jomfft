#pragma once

#include "../Dft.h"

#if USE_SINGLE
#define R_ABS fabsf
#define R_MIN fminf
#define R_MAX fmaxf
#define R_SIN sinf
#define R_COS cosf
#define R_SQRT sqrtf
#define C_REAL crealf
#define C_IMAG cimagf
#define C_ABS cabsf
#define C_CONJ conjf
#else
#define R_ABS fabs
#define R_MIN fmin
#define R_MAX fmax
#define R_SIN sin
#define R_COS cos
#define R_SQRT sqrt
#define C_REAL creal
#define C_IMAG cimag
#define C_ABS cabs
#define C_CONJ conj
#endif

#ifdef _MSC_VER
static inline Complex C_ADD(Complex x, Complex y) {
  return Complex_(C_REAL(x) + C_REAL(y), C_IMAG(x) + C_IMAG(y));
}
static inline Complex C_SUB(Complex x, Complex y) {
  return Complex_(C_REAL(x) - C_REAL(y), C_IMAG(x) - C_IMAG(y));
}
static inline Complex C_MUL(Complex x, Complex y) {
#if USE_SINGLE
  return _FCmulcc(x, y);
#else
  return _Cmulcc(x, y);
#endif
}
static inline Complex C_MUL_R(Complex x, Real y) {
#if USE_SINGLE
  return _FCmulcr(x, y);
#else
  return _Cmulcr(x, y);
#endif
}
static inline bool C_EQ(Complex x, Complex y) {
  return C_REAL(x) == C_REAL(y) && C_IMAG(x) == C_IMAG(y);
}
#else
static inline Complex C_ADD(Complex x, Complex y) { return x + y; }
static inline Complex C_SUB(Complex x, Complex y) { return x - y; }
static inline Complex C_MUL(Complex x, Complex y) { return x * y; }
static inline Complex C_MUL_R(Complex x, Real y) { return x * y; }
static inline bool C_EQ(Complex x, Complex y) { return x == y; }
#endif

#define C_SIGN(b) ((b) ? 1 : -1)

Complex NS(Complex_fromArgument)(Real theta);
