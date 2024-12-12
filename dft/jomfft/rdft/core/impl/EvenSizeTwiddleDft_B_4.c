#include "EvenSizeTwiddleDft_B.h"

#include "../Core.h"

#if C_SIMD_SIZE >= 4

#define CV_SIZE 4
#define FUNCTION NS(RDftCore_EvenSizeTwiddleDft_B_4)

#include "../template/EvenSizeTwiddleDft_B.h"

#endif
