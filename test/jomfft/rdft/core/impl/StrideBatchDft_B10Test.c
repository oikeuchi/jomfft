#include "StrideBatchDft_B10Test.h"

#include <jomfft/rdft/core/impl/StrideBatchDft_B10.h>

#include "../StrideBatchDftFunctionTest.h"

#if R_SIMD_SIZE_S >= 1
void NS(RDftCore_StrideBatchDft_B10_1Test)() {
  NS(RDftCore_StrideBatchDftFunction_BTest)
  (NS(RDftCore_StrideBatchDft_B10_1), 10, 8);
}
#endif

#if R_SIMD_SIZE_S >= 2
void NS(RDftCore_StrideBatchDft_B10_2Test)() {
  NS(RDftCore_StrideBatchDftFunction_BTest)
  (NS(RDftCore_StrideBatchDft_B10_2), 10, 8);
}
#endif

#if R_SIMD_SIZE_S >= 4
void NS(RDftCore_StrideBatchDft_B10_4Test)() {
  NS(RDftCore_StrideBatchDftFunction_BTest)
  (NS(RDftCore_StrideBatchDft_B10_4), 10, 8);
}
#endif

#if R_SIMD_SIZE_S >= 8
void NS(RDftCore_StrideBatchDft_B10_8Test)() {
  NS(RDftCore_StrideBatchDftFunction_BTest)
  (NS(RDftCore_StrideBatchDft_B10_8), 10, 8);
}
#endif
