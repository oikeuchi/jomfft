#include "EvenSizeTwiddleDft_F.h"

#include "../Core.h"

#if C_SIMD_SIZE >= 2

#define CV_SIZE 2
#define FUNCTION NS(RDftCore_EvenSizeTwiddleDft_F_2)

#include "../template/EvenSizeTwiddleDft_F.h"

#endif
