#pragma once

#include "../Util.h"

#if USE_SINGLE
#define MKL_PRECISION__ DFTI_SINGLE
#else
#define MKL_PRECISION__ DFTI_DOUBLE
#endif
