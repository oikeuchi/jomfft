#include "StrideBatchDft_F1.h"

#if R_SIMD_SIZE_S >= 1

#define RV_SIZE 1
#define FUNCTION NS(RDftSingleton_StrideBatchDft_F1_1)
#define TEMPLATE_FILE "Dft_F1.h"

#include "../template/StrideBatchDft_F.h"

#endif
