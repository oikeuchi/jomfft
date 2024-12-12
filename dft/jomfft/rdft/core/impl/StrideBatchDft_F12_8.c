#include "StrideBatchDft_F12.h"

#if R_SIMD_SIZE_S >= 8

#define RV_SIZE 8
#define FUNCTION NS(RDftCore_StrideBatchDft_F12_8)
#define TEMPLATE_FILE "Dft_F12.h"

#include "../template/StrideBatchDft_F.h"

#endif
