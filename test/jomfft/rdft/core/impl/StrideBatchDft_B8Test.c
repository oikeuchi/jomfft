#include "StrideBatchDft_B8Test.h"

#include <jomfft/rdft/core/impl/StrideBatchDft_B8.h>

#include "../StrideBatchDftFunctionTest.h"

#if R_SIMD_SIZE_S >= 1
void NS(RDftCore_StrideBatchDft_B8_1Test)() {
  NS(RDftCore_StrideBatchDftFunction_BTest)
  (NS(RDftCore_StrideBatchDft_B8_1), 8, 8);
}
#endif

#if R_SIMD_SIZE_S >= 2
void NS(RDftCore_StrideBatchDft_B8_2Test)() {
  NS(RDftCore_StrideBatchDftFunction_BTest)
  (NS(RDftCore_StrideBatchDft_B8_2), 8, 8);
}
#endif

#if R_SIMD_SIZE_S >= 4
void NS(RDftCore_StrideBatchDft_B8_4Test)() {
  NS(RDftCore_StrideBatchDftFunction_BTest)
  (NS(RDftCore_StrideBatchDft_B8_4), 8, 8);
}
#endif

#if R_SIMD_SIZE_S >= 8
void NS(RDftCore_StrideBatchDft_B8_8Test)() {
  NS(RDftCore_StrideBatchDftFunction_BTest)
  (NS(RDftCore_StrideBatchDft_B8_8), 8, 8);
}
#endif
