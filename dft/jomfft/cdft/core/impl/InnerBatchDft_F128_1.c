#include "InnerBatchDft_F128.h"

#if C_SIMD_SIZE >= 1

#define USE_COMPLEX_CONJ 0
#define CV_SIZE 1
#define FUNCTION NS(CDftCore_InnerBatchDft_F128_1)
#define TEMPLATE_FILE "Dft_128.h"

#include "../template/InnerBatchDft.h"

#endif