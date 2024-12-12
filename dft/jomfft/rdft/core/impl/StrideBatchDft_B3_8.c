#include "StrideBatchDft_B3.h"

#if R_SIMD_SIZE_S >= 8

#define RV_SIZE 8
#define FUNCTION NS(RDftCore_StrideBatchDft_B3_8)
#define TEMPLATE_FILE "Dft_B3.h"

#include "../template/StrideBatchDft_B.h"

#endif
