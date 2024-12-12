#include "InnerBatchDft_B1.h"

#if C_SIMD_SIZE >= 8

#define USE_COMPLEX_CONJ 1
#define CV_SIZE 8
#define FUNCTION NS(CDftSingleton_InnerBatchDft_B1_8)
#define TEMPLATE_FILE "Dft_1.h"

#include "../template/InnerBatchDft.h"

#endif
