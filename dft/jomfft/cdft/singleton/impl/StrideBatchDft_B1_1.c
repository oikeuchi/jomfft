#include "StrideBatchDft_B1.h"

#if C_SIMD_SIZE_S >= 1

#define USE_COMPLEX_CONJ 1
#define CV_SIZE 1
#define FUNCTION NS(CDftSingleton_StrideBatchDft_B1_1)
#define TEMPLATE_FILE "Dft_1.h"

#include "../template/StrideBatchDft.h"

#endif
