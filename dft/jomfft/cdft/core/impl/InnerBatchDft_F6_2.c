#include "InnerBatchDft_F6.h"

#if C_SIMD_SIZE >= 2

#define USE_COMPLEX_CONJ 0
#define CV_SIZE 2
#define FUNCTION NS(CDftCore_InnerBatchDft_F6_2)
#define TEMPLATE_FILE "Dft_6.h"

#include "../template/InnerBatchDft.h"

#endif