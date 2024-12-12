#include "StrideBatchDft_F32.h"

#if C_SIMD_SIZE_S >= 2

#define USE_COMPLEX_CONJ 0
#define CV_SIZE 2
#define FUNCTION NS(CDftCore_StrideBatchDft_F32_2)
#define TEMPLATE_FILE "Dft_32.h"

#include "../template/StrideBatchDft.h"

#endif
