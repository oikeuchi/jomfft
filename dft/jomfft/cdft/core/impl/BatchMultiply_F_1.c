#include "BatchMultiply_F.h"

#if C_SIMD_SIZE_S >= 1

#define USE_COMPLEX_CONJ 0
#define CV_SIZE 1
#define FUNCTION NS(CDftCore_BatchMultiply_F_1)

#include "../template/BatchMultiply.h"

#endif
