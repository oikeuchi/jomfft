#include "InnerBatchDft_B32.h"

#if C_SIMD_SIZE >= 8

#define USE_COMPLEX_CONJ 1
#define CV_SIZE 8
#define FUNCTION NS(CDftCore_InnerBatchDft_B32_8)
#define TEMPLATE_FILE "Dft_32.h"

#include "../template/InnerBatchDft.h"

#endif