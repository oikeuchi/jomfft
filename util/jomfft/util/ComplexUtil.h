#pragma once

#include "Util_.h"

bool NS(Complex_equal)(Complex lhs, Complex rhs);

bool NS(Complex_almostEqual)(Complex lhs, Complex rhs);

bool NS(Complex_almostEqual_)(Complex lhs, Complex rhs, Real abs_eps,
                              Real rel_eps);
