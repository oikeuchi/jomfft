#include "EvenSizeTwiddleDft_B.h"

#include "../Core.h"

#if C_SIMD_SIZE >= 8

#define CV_SIZE 8
#define FUNCTION NS(RDftCore_EvenSizeTwiddleDft_B_8)

#include "../template/EvenSizeTwiddleDft_B.h"

#endif
