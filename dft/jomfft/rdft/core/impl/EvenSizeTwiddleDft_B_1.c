#include "EvenSizeTwiddleDft_B.h"

#include "../Core.h"

#if C_SIMD_SIZE >= 1

#define CV_SIZE 1
#define FUNCTION NS(RDftCore_EvenSizeTwiddleDft_B_1)

#include "../template/EvenSizeTwiddleDft_B.h"

#endif
