#include "../../math/Math.h"

static FORCEINLINE CV_VECTOR CV_MUL_RI(CV_VECTOR x, CV_VECTOR y_r,
                                       CV_VECTOR y_i) {
  return RV_FMADDSUB(x, y_r, RV_MUL(RV_SWAP_LH(x), y_i));
}

static FORCEINLINE CV_VECTOR CV_MUL_RI_CONJ(CV_VECTOR x, CV_VECTOR y_r,
                                            CV_VECTOR y_i) {
  return RV_FMSUBADD(x, y_r, RV_MUL(RV_SWAP_LH(x), y_i));
}

static FORCEINLINE CV_VECTOR CV_CONJ_ADD(CV_VECTOR x, CV_VECTOR y) {
  return RV_FMSUBADD(RV_SCALAR(1), x, y);
}

static FORCEINLINE CV_VECTOR CV_CONJ_SUB(CV_VECTOR x, CV_VECTOR y) {
  return RV_ADDSUB(x, y);
}

static FORCEINLINE CV_VECTOR CV_MULW8(CV_VECTOR x) {
  const RV_VECTOR c = RV_SCALAR(M_SQRT1_2);
  return RV_FMSUBADD(c, x, RV_MUL(c, RV_SWAP_LH(x)));
}

static FORCEINLINE CV_VECTOR CV_MULW8_CONJ(CV_VECTOR x) {
  const RV_VECTOR c = RV_SCALAR(M_SQRT1_2);
  return RV_FMADDSUB(c, x, RV_MUL(c, RV_SWAP_LH(x)));
}

#include "../ComplexCommon.h"
