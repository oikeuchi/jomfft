#include "StrideBatchDft_B8.h"

#if R_SIMD_SIZE_S >= 4

#define RV_SIZE 4
#define FUNCTION NS(RDftCore_StrideBatchDft_B8_4)
#define TEMPLATE_FILE "Dft_B8.h"

#include "../template/StrideBatchDft_B.h"

#endif
