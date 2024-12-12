#include "StrideBatchDft_F1.h"

#if R_SIMD_SIZE_S >= 2

#define RV_SIZE 2
#define FUNCTION NS(RDftSingleton_StrideBatchDft_F1_2)
#define TEMPLATE_FILE "Dft_F1.h"

#include "../template/StrideBatchDft_F.h"

#endif
