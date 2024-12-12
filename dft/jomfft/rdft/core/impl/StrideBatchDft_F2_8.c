#include "StrideBatchDft_F2.h"

#if R_SIMD_SIZE_S >= 8

#define RV_SIZE 8
#define FUNCTION NS(RDftCore_StrideBatchDft_F2_8)
#define TEMPLATE_FILE "Dft_F2.h"

#include "../template/StrideBatchDft_F.h"

#endif
