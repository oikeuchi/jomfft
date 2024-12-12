#include "StrideBatchDft_F9.h"

#if R_SIMD_SIZE_S >= 2

#define RV_SIZE 2
#define FUNCTION NS(RDftCore_StrideBatchDft_F9_2)
#define TEMPLATE_FILE "Dft_F9.h"

#include "../template/StrideBatchDft_F.h"

#endif
