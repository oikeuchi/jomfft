#include "StrideBatchDft_F4.h"

#if C_SIMD_SIZE_S >= 1

#define USE_COMPLEX_CONJ 0
#define CV_SIZE 1
#define FUNCTION NS(CDftCore_StrideBatchDft_F4_1)
#define TEMPLATE_FILE "Dft_4.h"

#include "../template/StrideBatchDft.h"

#endif
