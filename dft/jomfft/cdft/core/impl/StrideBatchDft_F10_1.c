#include "StrideBatchDft_F10.h"

#if C_SIMD_SIZE_S >= 1

#define USE_COMPLEX_CONJ 0
#define CV_SIZE 1
#define FUNCTION NS(CDftCore_StrideBatchDft_F10_1)
#define TEMPLATE_FILE "Dft_10.h"

#include "../template/StrideBatchDft.h"

#endif
