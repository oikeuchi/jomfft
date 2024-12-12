#include "StrideBatchDft_F6.h"

#if R_SIMD_SIZE_S >= 1

#define RV_SIZE 1
#define FUNCTION NS(RDftCore_StrideBatchDft_F6_1)
#define TEMPLATE_FILE "Dft_F6.h"

#include "../template/StrideBatchDft_F.h"

#endif
