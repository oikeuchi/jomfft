#include "StrideBatchDft_B2Test.h"

#include <jomfft/rdft/core/impl/StrideBatchDft_B2.h>

#include "../StrideBatchDftFunctionTest.h"

#if R_SIMD_SIZE_S >= 1
void NS(RDftCore_StrideBatchDft_B2_1Test)() {
  NS(RDftCore_StrideBatchDftFunction_BTest)
  (NS(RDftCore_StrideBatchDft_B2_1), 2, 8);
}
#endif

#if R_SIMD_SIZE_S >= 2
void NS(RDftCore_StrideBatchDft_B2_2Test)() {
  NS(RDftCore_StrideBatchDftFunction_BTest)
  (NS(RDftCore_StrideBatchDft_B2_2), 2, 8);
}
#endif

#if R_SIMD_SIZE_S >= 4
void NS(RDftCore_StrideBatchDft_B2_4Test)() {
  NS(RDftCore_StrideBatchDftFunction_BTest)
  (NS(RDftCore_StrideBatchDft_B2_4), 2, 8);
}
#endif

#if R_SIMD_SIZE_S >= 8
void NS(RDftCore_StrideBatchDft_B2_8Test)() {
  NS(RDftCore_StrideBatchDftFunction_BTest)
  (NS(RDftCore_StrideBatchDft_B2_8), 2, 8);
}
#endif
