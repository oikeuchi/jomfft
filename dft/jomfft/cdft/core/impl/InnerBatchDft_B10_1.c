#include "InnerBatchDft_B10.h"

#if C_SIMD_SIZE >= 1

#define USE_COMPLEX_CONJ 1
#define CV_SIZE 1
#define FUNCTION NS(CDftCore_InnerBatchDft_B10_1)
#define TEMPLATE_FILE "Dft_10.h"

#include "../template/InnerBatchDft.h"

#endif
