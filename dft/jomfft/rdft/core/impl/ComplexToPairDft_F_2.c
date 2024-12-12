#include "ComplexToPairDft_F.h"

#include "../Core.h"

#if C_SIMD_SIZE_S >= 2

#define CV_SIZE 2
#define FUNCTION NS(RDftCore_ComplexToPairDft_F_2)

#include "../template/ComplexToPairDft_F.h"

#endif
