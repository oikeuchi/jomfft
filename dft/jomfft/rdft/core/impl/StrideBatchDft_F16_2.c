#include "StrideBatchDft_F16.h"

#if R_SIMD_SIZE_S >= 2

#define RV_SIZE 2
#define FUNCTION NS(RDftCore_StrideBatchDft_F16_2)
#define TEMPLATE_FILE "Dft_F16.h"

#include "../template/StrideBatchDft_F.h"

#endif
