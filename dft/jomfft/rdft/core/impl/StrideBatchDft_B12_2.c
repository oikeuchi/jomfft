#include "StrideBatchDft_B12.h"

#if R_SIMD_SIZE_S >= 2

#define RV_SIZE 2
#define FUNCTION NS(RDftCore_StrideBatchDft_B12_2)
#define TEMPLATE_FILE "Dft_B12.h"

#include "../template/StrideBatchDft_B.h"

#endif
