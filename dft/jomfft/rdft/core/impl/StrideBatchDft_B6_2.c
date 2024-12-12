#include "StrideBatchDft_B6.h"

#if R_SIMD_SIZE_S >= 2

#define RV_SIZE 2
#define FUNCTION NS(RDftCore_StrideBatchDft_B6_2)
#define TEMPLATE_FILE "Dft_B6.h"

#include "../template/StrideBatchDft_B.h"

#endif
