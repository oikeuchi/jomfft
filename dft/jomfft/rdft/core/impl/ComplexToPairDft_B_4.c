#include "ComplexToPairDft_B.h"

#include "../Core.h"

#if C_SIMD_SIZE_S >= 4

#define CV_SIZE 4
#define FUNCTION NS(RDftCore_ComplexToPairDft_B_4)

#include "../template/ComplexToPairDft_B.h"

#endif
