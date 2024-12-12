#include "StrideBatchDft_F0.h"

#if C_SIMD_SIZE_S >= 1

#define USE_COMPLEX_CONJ 0
#define CV_SIZE 1
#define FUNCTION NS(CDftSingleton_StrideBatchDft_F0_1)
#define TEMPLATE_FILE "Dft_0.h"

#include "../template/StrideBatchDft.h"

#endif
