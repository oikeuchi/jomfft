#include "StrideBatchDft_F3Test.h"

#include <jomfft/rdft/core/impl/StrideBatchDft_F3.h>

#include "../StrideBatchDftFunctionTest.h"

#if R_SIMD_SIZE_S >= 1
void NS(RDftCore_StrideBatchDft_F3_1Test)() {
  NS(RDftCore_StrideBatchDftFunction_FTest)
  (NS(RDftCore_StrideBatchDft_F3_1), 3, 8);
}
#endif

#if R_SIMD_SIZE_S >= 2
void NS(RDftCore_StrideBatchDft_F3_2Test)() {
  NS(RDftCore_StrideBatchDftFunction_FTest)
  (NS(RDftCore_StrideBatchDft_F3_2), 3, 8);
}
#endif

#if R_SIMD_SIZE_S >= 4
void NS(RDftCore_StrideBatchDft_F3_4Test)() {
  NS(RDftCore_StrideBatchDftFunction_FTest)
  (NS(RDftCore_StrideBatchDft_F3_4), 3, 8);
}
#endif

#if R_SIMD_SIZE_S >= 8
void NS(RDftCore_StrideBatchDft_F3_8Test)() {
  NS(RDftCore_StrideBatchDftFunction_FTest)
  (NS(RDftCore_StrideBatchDft_F3_8), 3, 8);
}
#endif
