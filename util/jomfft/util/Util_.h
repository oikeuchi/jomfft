#pragma once

#include "../Util.h"

#if USE_SINGLE
#define R_EPSILON 1.e-2f
#else
#define R_EPSILON 1.e-7
#endif

#define R_FORMAT "%.5f"

#define C_FORMAT "%.5f + %.5f I"
