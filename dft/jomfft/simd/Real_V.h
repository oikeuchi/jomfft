#include "Simd.h"

#ifdef SIMD_AVX
#include "x64/Real_V.h"
#elif defined(SIMD_NEON)
#include "arm/Real_V.h"
#else
#if RV_SIZE == 1
#include "Real_1.h"
#elif RV_SIZE == 2
#include "Real_2.h"
#endif
#endif
