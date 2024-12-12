#include "StrideBatchDft_B12.h"

#if R_SIMD_SIZE_S >= 1

#define RV_SIZE 1
#define FUNCTION NS(RDftCore_StrideBatchDft_B12_1)
#define TEMPLATE_FILE "Dft_B12.h"

#include "../template/StrideBatchDft_B.h"

#endif
