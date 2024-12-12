#include "StrideBatchDft_F64.h"

#if C_SIMD_SIZE_S >= 2

#define USE_COMPLEX_CONJ 0
#define CV_SIZE 2
#define FUNCTION NS(CDftCore_StrideBatchDft_F64_2)
#define TEMPLATE_FILE "Dft_64.h"

#include "../template/StrideBatchDft.h"

#endif
