#include "EvenSizeTwiddleDft_F.h"

#include "../Core.h"

#if C_SIMD_SIZE >= 1

#define CV_SIZE 1
#define FUNCTION NS(RDftCore_EvenSizeTwiddleDft_F_1)

#include "../template/EvenSizeTwiddleDft_F.h"

#endif
