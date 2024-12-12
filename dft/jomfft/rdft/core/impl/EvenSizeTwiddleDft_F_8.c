#include "EvenSizeTwiddleDft_F.h"

#include "../Core.h"

#if C_SIMD_SIZE >= 8

#define CV_SIZE 8
#define FUNCTION NS(RDftCore_EvenSizeTwiddleDft_F_8)

#include "../template/EvenSizeTwiddleDft_F.h"

#endif
