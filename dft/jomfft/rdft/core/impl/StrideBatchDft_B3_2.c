#include "StrideBatchDft_B3.h"

#if R_SIMD_SIZE_S >= 2

#define RV_SIZE 2
#define FUNCTION NS(RDftCore_StrideBatchDft_B3_2)
#define TEMPLATE_FILE "Dft_B3.h"

#include "../template/StrideBatchDft_B.h"

#endif