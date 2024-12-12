#include "StrideBatchDft_B12.h"

#if C_SIMD_SIZE_S >= 2

#define USE_COMPLEX_CONJ 1
#define CV_SIZE 2
#define FUNCTION NS(CDftCore_StrideBatchDft_B12_2)
#define TEMPLATE_FILE "Dft_12.h"

#include "../template/StrideBatchDft.h"

#endif
