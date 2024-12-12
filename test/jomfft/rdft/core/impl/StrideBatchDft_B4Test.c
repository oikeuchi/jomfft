#include "StrideBatchDft_B4Test.h"

#include <jomfft/rdft/core/impl/StrideBatchDft_B4.h>

#include "../StrideBatchDftFunctionTest.h"

#if R_SIMD_SIZE_S >= 1
void NS(RDftCore_StrideBatchDft_B4_1Test)() {
  NS(RDftCore_StrideBatchDftFunction_BTest)
  (NS(RDftCore_StrideBatchDft_B4_1), 4, 8);
}
#endif

#if R_SIMD_SIZE_S >= 2
void NS(RDftCore_StrideBatchDft_B4_2Test)() {
  NS(RDftCore_StrideBatchDftFunction_BTest)
  (NS(RDftCore_StrideBatchDft_B4_2), 4, 8);
}
#endif

#if R_SIMD_SIZE_S >= 4
void NS(RDftCore_StrideBatchDft_B4_4Test)() {
  NS(RDftCore_StrideBatchDftFunction_BTest)
  (NS(RDftCore_StrideBatchDft_B4_4), 4, 8);
}
#endif

#if R_SIMD_SIZE_S >= 8
void NS(RDftCore_StrideBatchDft_B4_8Test)() {
  NS(RDftCore_StrideBatchDftFunction_BTest)
  (NS(RDftCore_StrideBatchDft_B4_8), 4, 8);
}
#endif
