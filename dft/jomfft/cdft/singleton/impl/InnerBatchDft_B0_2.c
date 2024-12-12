#include "InnerBatchDft_B0.h"

#if C_SIMD_SIZE >= 2

#define USE_COMPLEX_CONJ 1
#define CV_SIZE 2
#define FUNCTION NS(CDftSingleton_InnerBatchDft_B0_2)
#define TEMPLATE_FILE "Dft_0.h"

#include "../template/InnerBatchDft.h"

#endif
