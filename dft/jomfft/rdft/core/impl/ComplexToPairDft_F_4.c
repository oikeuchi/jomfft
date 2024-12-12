#include "ComplexToPairDft_F.h"

#include "../Core.h"

#if C_SIMD_SIZE_S >= 4

#define CV_SIZE 4
#define FUNCTION NS(RDftCore_ComplexToPairDft_F_4)

#include "../template/ComplexToPairDft_F.h"

#endif
