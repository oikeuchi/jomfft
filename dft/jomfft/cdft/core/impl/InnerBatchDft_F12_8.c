#include "InnerBatchDft_F12.h"

#if C_SIMD_SIZE >= 8

#define USE_COMPLEX_CONJ 0
#define CV_SIZE 8
#define FUNCTION NS(CDftCore_InnerBatchDft_F12_8)
#define TEMPLATE_FILE "Dft_12.h"

#include "../template/InnerBatchDft.h"

#endif