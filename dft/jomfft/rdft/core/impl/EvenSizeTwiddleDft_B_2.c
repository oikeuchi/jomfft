#include "EvenSizeTwiddleDft_B.h"

#include "../Core.h"

#if C_SIMD_SIZE >= 2

#define CV_SIZE 2
#define FUNCTION NS(RDftCore_EvenSizeTwiddleDft_B_2)

#include "../template/EvenSizeTwiddleDft_B.h"

#endif
