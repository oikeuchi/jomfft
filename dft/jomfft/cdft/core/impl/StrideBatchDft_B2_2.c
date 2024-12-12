#include "StrideBatchDft_B2.h"

#if C_SIMD_SIZE_S >= 2

#define USE_COMPLEX_CONJ 1
#define CV_SIZE 2
#define FUNCTION NS(CDftCore_StrideBatchDft_B2_2)
#define TEMPLATE_FILE "Dft_2.h"

#include "../template/StrideBatchDft.h"

#endif
