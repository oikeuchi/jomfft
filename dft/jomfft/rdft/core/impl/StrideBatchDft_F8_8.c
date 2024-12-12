#include "StrideBatchDft_F8.h"

#if R_SIMD_SIZE_S >= 8

#define RV_SIZE 8
#define FUNCTION NS(RDftCore_StrideBatchDft_F8_8)
#define TEMPLATE_FILE "Dft_F8.h"

#include "../template/StrideBatchDft_F.h"

#endif
