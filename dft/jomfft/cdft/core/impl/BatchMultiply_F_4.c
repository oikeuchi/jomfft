#include "BatchMultiply_F.h"

#if C_SIMD_SIZE_S >= 4

#define USE_COMPLEX_CONJ 0
#define CV_SIZE 4
#define FUNCTION NS(CDftCore_BatchMultiply_F_4)

#include "../template/BatchMultiply.h"

#endif
