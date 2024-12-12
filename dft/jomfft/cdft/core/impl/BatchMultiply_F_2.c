#include "BatchMultiply_F.h"

#if C_SIMD_SIZE_S >= 2

#define USE_COMPLEX_CONJ 0
#define CV_SIZE 2
#define FUNCTION NS(CDftCore_BatchMultiply_F_2)

#include "../template/BatchMultiply.h"

#endif
