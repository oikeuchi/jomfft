#pragma once

#include "Util_.h"

bool NS(Real_equal)(Real lhs, Real rhs);

bool NS(Real_almostEqual)(Real lhs, Real rhs);

bool NS(Real_almostEqual_)(Real lhs, Real rhs, Real abs_eps, Real rel_eps);
