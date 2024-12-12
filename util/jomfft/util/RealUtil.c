#include "RealUtil.h"

#include <jomfft/math/Math.h>

bool NS(Real_equal)(Real lhs, Real rhs) { return lhs == rhs; }

bool NS(Real_almostEqual)(Real lhs, Real rhs) {
  return NS(Real_almostEqual_)(lhs, rhs, R_EPSILON, R_EPSILON);
}

bool NS(Real_almostEqual_)(Real lhs, Real rhs, Real abs_eps, Real rel_eps) {
  return R_ABS(lhs - rhs) <=
         R_MAX(abs_eps, rel_eps * R_MAX(R_ABS(lhs), R_ABS(rhs)));
}
