#include "StrideBatchDft_F128.h"

#if R_SIMD_SIZE_S >= 1

#define RV_SIZE 1
#define FUNCTION NS(RDftCore_StrideBatchDft_F128_1)
#define TEMPLATE_FILE "Dft_F128.h"

#include "../template/StrideBatchDft_F.h"

#endif