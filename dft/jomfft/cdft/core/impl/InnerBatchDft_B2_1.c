#include "InnerBatchDft_B2.h"

#if C_SIMD_SIZE >= 1

#define USE_COMPLEX_CONJ 1
#define CV_SIZE 1
#define FUNCTION NS(CDftCore_InnerBatchDft_B2_1)
#define TEMPLATE_FILE "Dft_2.h"

#include "../template/InnerBatchDft.h"

#endif