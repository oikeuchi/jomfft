#include "InnerBatchDft_B12.h"

#if C_SIMD_SIZE >= 4

#define USE_COMPLEX_CONJ 1
#define CV_SIZE 4
#define FUNCTION NS(CDftCore_InnerBatchDft_B12_4)
#define TEMPLATE_FILE "Dft_12.h"

#include "../template/InnerBatchDft.h"

#endif