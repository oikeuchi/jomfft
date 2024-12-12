#include "StrideBatchDft_F0.h"

#if R_SIMD_SIZE_S >= 2

#define RV_SIZE 2
#define FUNCTION NS(RDftSingleton_StrideBatchDft_F0_2)
#define TEMPLATE_FILE "Dft_F0.h"

#include "../template/StrideBatchDft_F.h"

#endif
