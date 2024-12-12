#include "StrideBatchDft_B4.h"

#if R_SIMD_SIZE_S >= 2

#define RV_SIZE 2
#define FUNCTION NS(RDftCore_StrideBatchDft_B4_2)
#define TEMPLATE_FILE "Dft_B4.h"

#include "../template/StrideBatchDft_B.h"

#endif
