#include "EvenSizeTwiddleDft_F.h"

#include "../Core.h"

#if C_SIMD_SIZE >= 4

#define CV_SIZE 4
#define FUNCTION NS(RDftCore_EvenSizeTwiddleDft_F_4)

#include "../template/EvenSizeTwiddleDft_F.h"

#endif
