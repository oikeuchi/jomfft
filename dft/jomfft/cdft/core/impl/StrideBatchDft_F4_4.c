#include "StrideBatchDft_F4.h"

#if C_SIMD_SIZE_S >= 4

#define USE_COMPLEX_CONJ 0
#define CV_SIZE 4
#define FUNCTION NS(CDftCore_StrideBatchDft_F4_4)
#define TEMPLATE_FILE "Dft_4.h"

#include "../template/StrideBatchDft.h"

#endif
