#include "StrideBatchDft_B32.h"

#if R_SIMD_SIZE_S >= 4

#define RV_SIZE 4
#define FUNCTION NS(RDftCore_StrideBatchDft_B32_4)
#define TEMPLATE_FILE "Dft_B32.h"

#include "../template/StrideBatchDft_B.h"

#endif
