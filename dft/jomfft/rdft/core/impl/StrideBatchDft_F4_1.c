#include "StrideBatchDft_F4.h"

#if R_SIMD_SIZE_S >= 1

#define RV_SIZE 1
#define FUNCTION NS(RDftCore_StrideBatchDft_F4_1)
#define TEMPLATE_FILE "Dft_F4.h"

#include "../template/StrideBatchDft_F.h"

#endif
