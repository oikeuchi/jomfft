#include "StrideBatchDft_F7Test.h"

#include <jomfft/rdft/core/impl/StrideBatchDft_F7.h>

#include "../StrideBatchDftFunctionTest.h"

#if R_SIMD_SIZE_S >= 1
void NS(RDftCore_StrideBatchDft_F7_1Test)() {
  NS(RDftCore_StrideBatchDftFunction_FTest)
  (NS(RDftCore_StrideBatchDft_F7_1), 7, 8);
}
#endif

#if R_SIMD_SIZE_S >= 2
void NS(RDftCore_StrideBatchDft_F7_2Test)() {
  NS(RDftCore_StrideBatchDftFunction_FTest)
  (NS(RDftCore_StrideBatchDft_F7_2), 7, 8);
}
#endif

#if R_SIMD_SIZE_S >= 4
void NS(RDftCore_StrideBatchDft_F7_4Test)() {
  NS(RDftCore_StrideBatchDftFunction_FTest)
  (NS(RDftCore_StrideBatchDft_F7_4), 7, 8);
}
#endif

#if R_SIMD_SIZE_S >= 8
void NS(RDftCore_StrideBatchDft_F7_8Test)() {
  NS(RDftCore_StrideBatchDftFunction_FTest)
  (NS(RDftCore_StrideBatchDft_F7_8), 7, 8);
}
#endif
