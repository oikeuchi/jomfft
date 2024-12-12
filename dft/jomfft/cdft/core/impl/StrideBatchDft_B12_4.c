#include "StrideBatchDft_B12.h"

#if C_SIMD_SIZE_S >= 4

#define USE_COMPLEX_CONJ 1
#define CV_SIZE 4
#define FUNCTION NS(CDftCore_StrideBatchDft_B12_4)
#define TEMPLATE_FILE "Dft_12.h"

#include "../template/StrideBatchDft.h"

#endif
