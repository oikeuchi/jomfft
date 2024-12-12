#include "StrideBatchDft_F16.h"

#if R_SIMD_SIZE_S >= 4

#define RV_SIZE 4
#define FUNCTION NS(RDftCore_StrideBatchDft_F16_4)
#define TEMPLATE_FILE "Dft_F16.h"

#include "../template/StrideBatchDft_F.h"

#endif
