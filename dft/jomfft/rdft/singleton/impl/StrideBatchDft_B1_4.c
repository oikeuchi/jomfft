#include "StrideBatchDft_B1.h"

#if R_SIMD_SIZE_S >= 4

#define RV_SIZE 4
#define FUNCTION NS(RDftSingleton_StrideBatchDft_B1_4)
#define TEMPLATE_FILE "Dft_B1.h"

#include "../template/StrideBatchDft_B.h"

#endif
