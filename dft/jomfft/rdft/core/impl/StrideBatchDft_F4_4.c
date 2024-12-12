#include "StrideBatchDft_F4.h"

#if R_SIMD_SIZE_S >= 4

#define RV_SIZE 4
#define FUNCTION NS(RDftCore_StrideBatchDft_F4_4)
#define TEMPLATE_FILE "Dft_F4.h"

#include "../template/StrideBatchDft_F.h"

#endif
