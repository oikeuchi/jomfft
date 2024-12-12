#include "StrideBatchDft_F10.h"

#if R_SIMD_SIZE_S >= 1

#define RV_SIZE 1
#define FUNCTION NS(RDftCore_StrideBatchDft_F10_1)
#define TEMPLATE_FILE "Dft_F10.h"

#include "../template/StrideBatchDft_F.h"

#endif
