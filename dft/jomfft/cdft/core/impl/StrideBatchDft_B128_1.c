#include "StrideBatchDft_B128.h"

#if C_SIMD_SIZE_S >= 1

#define USE_COMPLEX_CONJ 1
#define CV_SIZE 1
#define FUNCTION NS(CDftCore_StrideBatchDft_B128_1)
#define TEMPLATE_FILE "Dft_128.h"

#include "../template/StrideBatchDft.h"

#endif
