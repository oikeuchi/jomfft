#include "StrideBatchDft_F0.h"

#if R_SIMD_SIZE_S >= 8

#define RV_SIZE 8
#define FUNCTION NS(RDftSingleton_StrideBatchDft_F0_8)
#define TEMPLATE_FILE "Dft_F0.h"

#include "../template/StrideBatchDft_F.h"

#endif
