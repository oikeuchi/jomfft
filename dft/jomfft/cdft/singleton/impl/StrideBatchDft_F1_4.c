#include "StrideBatchDft_F1.h"

#if C_SIMD_SIZE_S >= 4

#define USE_COMPLEX_CONJ 0
#define CV_SIZE 4
#define FUNCTION NS(CDftSingleton_StrideBatchDft_F1_4)
#define TEMPLATE_FILE "Dft_1.h"

#include "../template/StrideBatchDft.h"

#endif
