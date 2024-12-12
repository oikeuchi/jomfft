#include "../../math/Math.h"

static FORCEINLINE CV_VECTOR CV_MUL_RI(CV_VECTOR x, CV_VECTOR y_r,
                                       CV_VECTOR y_i) {
  CV_VECTOR s = RV_SWAP_LH(CV_CONJ(RV_MUL(x, y_i)));
  return RV_FMADD(x, y_r, s);
}

static FORCEINLINE CV_VECTOR CV_MUL_RI_CONJ(CV_VECTOR x, CV_VECTOR y_r,
                                            CV_VECTOR y_i) {
  CV_VECTOR s = CV_CONJ(RV_SWAP_LH(RV_MUL(x, y_i)));
  return RV_FMADD(x, y_r, s);
}

static FORCEINLINE CV_VECTOR CV_CONJ_ADD(CV_VECTOR x, CV_VECTOR y) {
  return RV_ADD(x, CV_CONJ(y));
}

static FORCEINLINE CV_VECTOR CV_CONJ_SUB(CV_VECTOR x, CV_VECTOR y) {
  return RV_SUB(x, CV_CONJ(y));
}

static FORCEINLINE CV_VECTOR CV_MULW8(CV_VECTOR x) {
  x = CV_CONJ_ADD(x, RV_SWAP_LH(x));
  return RV_MUL(x, RV_SCALAR(M_SQRT1_2));
}

static FORCEINLINE CV_VECTOR CV_MULW8_CONJ(CV_VECTOR x) {
  x = CV_CONJ_SUB(x, RV_SWAP_LH(x));
  return RV_MUL(x, RV_SCALAR(M_SQRT1_2));
}

#include "../ComplexCommon.h"
