#include "StrideBatchDft_B2.h"

#if C_SIMD_SIZE_S >= 4

#define USE_COMPLEX_CONJ 1
#define CV_SIZE 4
#define FUNCTION NS(CDftCore_StrideBatchDft_B2_4)
#define TEMPLATE_FILE "Dft_2.h"

#include "../template/StrideBatchDft.h"

#endif