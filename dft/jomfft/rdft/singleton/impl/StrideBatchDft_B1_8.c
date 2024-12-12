#include "StrideBatchDft_B1.h"

#if R_SIMD_SIZE_S >= 8

#define RV_SIZE 8
#define FUNCTION NS(RDftSingleton_StrideBatchDft_B1_8)
#define TEMPLATE_FILE "Dft_B1.h"

#include "../template/StrideBatchDft_B.h"

#endif
