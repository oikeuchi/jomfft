#include "InnerBatchDft_F16.h"

#if C_SIMD_SIZE >= 4

#define USE_COMPLEX_CONJ 0
#define CV_SIZE 4
#define FUNCTION NS(CDftCore_InnerBatchDft_F16_4)
#define TEMPLATE_FILE "Dft_16.h"

#include "../template/InnerBatchDft.h"

#endif