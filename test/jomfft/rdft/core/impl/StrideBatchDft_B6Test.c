#include "StrideBatchDft_B6Test.h"

#include <jomfft/rdft/core/impl/StrideBatchDft_B6.h>

#include "../StrideBatchDftFunctionTest.h"

#if R_SIMD_SIZE_S >= 1
void NS(RDftCore_StrideBatchDft_B6_1Test)() {
  NS(RDftCore_StrideBatchDftFunction_BTest)
  (NS(RDftCore_StrideBatchDft_B6_1), 6, 8);
}
#endif

#if R_SIMD_SIZE_S >= 2
void NS(RDftCore_StrideBatchDft_B6_2Test)() {
  NS(RDftCore_StrideBatchDftFunction_BTest)
  (NS(RDftCore_StrideBatchDft_B6_2), 6, 8);
}
#endif

#if R_SIMD_SIZE_S >= 4
void NS(RDftCore_StrideBatchDft_B6_4Test)() {
  NS(RDftCore_StrideBatchDftFunction_BTest)
  (NS(RDftCore_StrideBatchDft_B6_4), 6, 8);
}
#endif

#if R_SIMD_SIZE_S >= 8
void NS(RDftCore_StrideBatchDft_B6_8Test)() {
  NS(RDftCore_StrideBatchDftFunction_BTest)
  (NS(RDftCore_StrideBatchDft_B6_8), 6, 8);
}
#endif
