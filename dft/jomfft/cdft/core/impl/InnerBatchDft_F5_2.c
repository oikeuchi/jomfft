#include "InnerBatchDft_F5.h"

#if C_SIMD_SIZE >= 2

#define USE_COMPLEX_CONJ 0
#define CV_SIZE 2
#define FUNCTION NS(CDftCore_InnerBatchDft_F5_2)
#define TEMPLATE_FILE "Dft_5.h"

#include "../template/InnerBatchDft.h"

#endif