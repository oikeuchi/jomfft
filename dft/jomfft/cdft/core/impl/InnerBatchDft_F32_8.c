#include "InnerBatchDft_F32.h"

#if C_SIMD_SIZE >= 8

#define USE_COMPLEX_CONJ 0
#define CV_SIZE 8
#define FUNCTION NS(CDftCore_InnerBatchDft_F32_8)
#define TEMPLATE_FILE "Dft_32.h"

#include "../template/InnerBatchDft.h"

#endif
