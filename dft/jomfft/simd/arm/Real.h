static FORCEINLINE RV_VECTOR RV_FMADD(RV_VECTOR x, RV_VECTOR y, RV_VECTOR z) {
  return RV_MLA__(z, x, y);
}

static FORCEINLINE RV_VECTOR RV_FNMADD(RV_VECTOR x, RV_VECTOR y, RV_VECTOR z) {
  return RV_MLS__(z, x, y);
}

static FORCEINLINE RV_VECTOR RV_FMSUB(RV_VECTOR x, RV_VECTOR y, RV_VECTOR z) {
  return RV_SUB(RV_MUL(x, y), z);
}

#include "../RealCommon.h"
