#include "StrideBatchDft_B7.h"

#if R_SIMD_SIZE_S >= 8

#define RV_SIZE 8
#define FUNCTION NS(RDftCore_StrideBatchDft_B7_8)
#define TEMPLATE_FILE "Dft_B7.h"

#include "../template/StrideBatchDft_B.h"

#endif
