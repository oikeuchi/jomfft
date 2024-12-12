#include "InnerBatchDft_B9.h"

#if C_SIMD_SIZE >= 8

#define USE_COMPLEX_CONJ 1
#define CV_SIZE 8
#define FUNCTION NS(CDftCore_InnerBatchDft_B9_8)
#define TEMPLATE_FILE "Dft_9.h"

#include "../template/InnerBatchDft.h"

#endif
