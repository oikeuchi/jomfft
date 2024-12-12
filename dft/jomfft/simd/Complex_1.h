#pragma once

#include "Real_2.h"

#define CV_VECTOR RV_VECTOR

static FORCEINLINE CV_VECTOR CV_LOAD_S(const Complex *p, Long s) {
  (void)s;
  return *p;
}

static FORCEINLINE void CV_STORE_S(Complex *p, Long s, CV_VECTOR x) {
  (void)s;
  *p = x;
}

#define CV_CONJ C_CONJ

static FORCEINLINE CV_VECTOR CV_REVERSE(CV_VECTOR x) { return x; }

static FORCEINLINE CV_VECTOR CV_MUL_RI(CV_VECTOR x, CV_VECTOR y_r,
                                       CV_VECTOR y_i) {
  const Complex c = RV_MUL(RV_SWAP_LH(x), y_i);
  return Complex_(C_REAL(x) * C_REAL(y_r) - C_REAL(c),
                  C_IMAG(x) * C_IMAG(y_r) + C_IMAG(c));
}

static FORCEINLINE CV_VECTOR CV_MUL_RI_CONJ(CV_VECTOR x, CV_VECTOR y_r,
                                            CV_VECTOR y_i) {
  const Complex c = RV_MUL(RV_SWAP_LH(x), y_i);
  return Complex_(C_REAL(x) * C_REAL(y_r) + C_REAL(c),
                  C_IMAG(x) * C_IMAG(y_r) - C_IMAG(c));
}

static FORCEINLINE CV_VECTOR CV_CONJ_ADD(CV_VECTOR x, CV_VECTOR y) {
  return C_ADD(x, C_CONJ(y));
}

static FORCEINLINE CV_VECTOR CV_CONJ_SUB(CV_VECTOR x, CV_VECTOR y) {
  return C_SUB(x, C_CONJ(y));
}

static FORCEINLINE CV_VECTOR CV_MULW8(CV_VECTOR x) {
  const Real x_r = C_REAL(x);
  const Real x_i = C_IMAG(x);
  return C_MUL_R(Complex_(x_r + x_i, x_i - x_r), (Real)M_SQRT1_2);
}

static FORCEINLINE CV_VECTOR CV_MULW8_CONJ(CV_VECTOR x) {
  const Real x_r = C_REAL(x);
  const Real x_i = C_IMAG(x);
  return C_MUL_R(Complex_(x_r - x_i, x_i + x_r), (Real)M_SQRT1_2);
}

static FORCEINLINE CV_VECTOR CV_LOAD(const Complex *p) {
  return RV_LOAD((const Real *)p);
}

static FORCEINLINE void CV_STORE(Complex *p, CV_VECTOR x) {
  RV_STORE((Real *)p, x);
}

#define CV_ZERO RV_ZERO
#define CV_ADD RV_ADD
#define CV_SUB RV_SUB

static FORCEINLINE CV_VECTOR CV_MUL(CV_VECTOR x, CV_VECTOR y) {
  return C_MUL(x, y);
}

static FORCEINLINE CV_VECTOR CV_MUL_CONJ(CV_VECTOR x, CV_VECTOR y) {
  return C_MUL(x, C_CONJ(y));
}

static FORCEINLINE CV_VECTOR CV_MUL_SCALAR(CV_VECTOR x, const Complex y) {
  return C_MUL(x, y);
}

static FORCEINLINE CV_VECTOR CV_MUL_SCALAR_CONJ(CV_VECTOR x, const Complex y) {
  return C_MUL(x, C_CONJ(y));
}

static FORCEINLINE CV_VECTOR CV_MULI(CV_VECTOR x) {
  return Complex_(-C_IMAG(x), C_REAL(x));
}

static FORCEINLINE CV_VECTOR CV_MULI_CONJ(CV_VECTOR x) {
  return Complex_(C_IMAG(x), -C_REAL(x));
}

static FORCEINLINE CV_VECTOR CV_MULI_ADD(CV_VECTOR x, CV_VECTOR y) {
  return C_ADD(x, CV_MULI(y));
}

static FORCEINLINE CV_VECTOR CV_MULI_SUB(CV_VECTOR x, CV_VECTOR y) {
  return C_SUB(x, CV_MULI(y));
}

#define CV_MULI_ADD_CONJ CV_MULI_SUB
#define CV_MULI_SUB_CONJ CV_MULI_ADD

static FORCEINLINE void CV_DFT2(CV_VECTOR *x0, CV_VECTOR *x1) {
  CV_VECTOR t = C_ADD(*x0, *x1);
  *x1 = C_SUB(*x0, *x1);
  *x0 = t;
}

static FORCEINLINE void CV_CONJ_DFT2(CV_VECTOR *x0, CV_VECTOR *x1) {
  CV_VECTOR t = CV_CONJ_ADD(*x0, *x1);
  *x1 = CV_CONJ_SUB(*x0, *x1);
  *x0 = t;
}

static FORCEINLINE void CV_MULI_DFT2(CV_VECTOR *x0, CV_VECTOR *x1) {
  CV_VECTOR t = CV_MULI_ADD(*x0, *x1);
  *x1 = CV_MULI_SUB(*x0, *x1);
  *x0 = t;
}

static FORCEINLINE void CV_MULI_DFT2_CONJ(CV_VECTOR *x0, CV_VECTOR *x1) {
  CV_VECTOR t = CV_MULI_ADD_CONJ(*x0, *x1);
  *x1 = CV_MULI_SUB_CONJ(*x0, *x1);
  *x0 = t;
}
