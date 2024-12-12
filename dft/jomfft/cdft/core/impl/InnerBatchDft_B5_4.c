#include "InnerBatchDft_B5.h"

#if C_SIMD_SIZE >= 4

#define USE_COMPLEX_CONJ 1
#define CV_SIZE 4
#define FUNCTION NS(CDftCore_InnerBatchDft_B5_4)
#define TEMPLATE_FILE "Dft_5.h"

#include "../template/InnerBatchDft.h"

#endif
