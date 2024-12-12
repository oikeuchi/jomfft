#include "StrideBatchDft_F1.h"

#if R_SIMD_SIZE_S >= 4

#define RV_SIZE 4
#define FUNCTION NS(RDftSingleton_StrideBatchDft_F1_4)
#define TEMPLATE_FILE "Dft_F1.h"

#include "../template/StrideBatchDft_F.h"

#endif
