#include "ComplexUtil.h"

#include <jomfft/math/Math.h>

bool NS(Complex_equal)(Complex lhs, Complex rhs) { return C_EQ(lhs, rhs); }

bool NS(Complex_almostEqual)(Complex lhs, Complex rhs) {
  return NS(Complex_almostEqual_)(lhs, rhs, R_EPSILON, R_EPSILON);
}

bool NS(Complex_almostEqual_)(Complex lhs, Complex rhs, Real abs_eps,
                              Real rel_eps) {
  return C_ABS(C_SUB(lhs, rhs)) <=
         R_MAX(abs_eps, rel_eps * R_MAX(C_ABS(lhs), C_ABS(rhs)));
}
