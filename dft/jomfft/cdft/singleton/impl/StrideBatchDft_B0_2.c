#include "StrideBatchDft_B0.h"

#if C_SIMD_SIZE_S >= 2

#define USE_COMPLEX_CONJ 1
#define CV_SIZE 2
#define FUNCTION NS(CDftSingleton_StrideBatchDft_B0_2)
#define TEMPLATE_FILE "Dft_0.h"

#include "../template/StrideBatchDft.h"

#endif