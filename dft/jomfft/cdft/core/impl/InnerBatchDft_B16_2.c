#include "InnerBatchDft_B16.h"

#if C_SIMD_SIZE >= 2

#define USE_COMPLEX_CONJ 1
#define CV_SIZE 2
#define FUNCTION NS(CDftCore_InnerBatchDft_B16_2)
#define TEMPLATE_FILE "Dft_16.h"

#include "../template/InnerBatchDft.h"

#endif