#include "ComplexToPairDft_B.h"

#include "../Core.h"

#if C_SIMD_SIZE_S >= 1

#define CV_SIZE 1
#define FUNCTION NS(RDftCore_ComplexToPairDft_B_1)

#include "../template/ComplexToPairDft_B.h"

#endif
