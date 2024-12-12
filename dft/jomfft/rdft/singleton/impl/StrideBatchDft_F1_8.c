#include "StrideBatchDft_F1.h"

#if R_SIMD_SIZE_S >= 8

#define RV_SIZE 8
#define FUNCTION NS(RDftSingleton_StrideBatchDft_F1_8)
#define TEMPLATE_FILE "Dft_F1.h"

#include "../template/StrideBatchDft_F.h"

#endif
