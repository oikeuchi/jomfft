#include "StrideBatchDft_B0.h"

#if R_SIMD_SIZE_S >= 4

#define RV_SIZE 4
#define FUNCTION NS(RDftSingleton_StrideBatchDft_B0_4)
#define TEMPLATE_FILE "Dft_B0.h"

#include "../template/StrideBatchDft_B.h"

#endif
