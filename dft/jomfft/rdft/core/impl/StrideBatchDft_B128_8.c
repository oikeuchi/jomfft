#include "StrideBatchDft_B128.h"

#if R_SIMD_SIZE_S >= 8

#define RV_SIZE 8
#define FUNCTION NS(RDftCore_StrideBatchDft_B128_8)
#define TEMPLATE_FILE "Dft_B128.h"

#include "../template/StrideBatchDft_B.h"

#endif
