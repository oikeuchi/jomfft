#include "StrideBatchDft_B128.h"

#if R_SIMD_SIZE_S >= 1

#define RV_SIZE 1
#define FUNCTION NS(RDftCore_StrideBatchDft_B128_1)
#define TEMPLATE_FILE "Dft_B128.h"

#include "../template/StrideBatchDft_B.h"

#endif
