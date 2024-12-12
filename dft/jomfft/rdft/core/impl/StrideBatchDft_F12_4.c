#include "StrideBatchDft_F12.h"

#if R_SIMD_SIZE_S >= 4

#define RV_SIZE 4
#define FUNCTION NS(RDftCore_StrideBatchDft_F12_4)
#define TEMPLATE_FILE "Dft_F12.h"

#include "../template/StrideBatchDft_F.h"

#endif
