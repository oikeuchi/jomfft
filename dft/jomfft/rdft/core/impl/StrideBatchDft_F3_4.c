#include "StrideBatchDft_F3.h"

#if R_SIMD_SIZE_S >= 4

#define RV_SIZE 4
#define FUNCTION NS(RDftCore_StrideBatchDft_F3_4)
#define TEMPLATE_FILE "Dft_F3.h"

#include "../template/StrideBatchDft_F.h"

#endif