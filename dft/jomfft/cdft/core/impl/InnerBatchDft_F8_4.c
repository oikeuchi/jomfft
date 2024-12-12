#include "InnerBatchDft_F8.h"

#if C_SIMD_SIZE >= 4

#define USE_COMPLEX_CONJ 0
#define CV_SIZE 4
#define FUNCTION NS(CDftCore_InnerBatchDft_F8_4)
#define TEMPLATE_FILE "Dft_8.h"

#include "../template/InnerBatchDft.h"

#endif
