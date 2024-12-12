#include "StrideBatchDft_F4Test.h"

#include <jomfft/rdft/core/impl/StrideBatchDft_F4.h>

#include "../StrideBatchDftFunctionTest.h"

#if R_SIMD_SIZE_S >= 1
void NS(RDftCore_StrideBatchDft_F4_1Test)() {
  NS(RDftCore_StrideBatchDftFunction_FTest)
  (NS(RDftCore_StrideBatchDft_F4_1), 4, 8);
}
#endif

#if R_SIMD_SIZE_S >= 2
void NS(RDftCore_StrideBatchDft_F4_2Test)() {
  NS(RDftCore_StrideBatchDftFunction_FTest)
  (NS(RDftCore_StrideBatchDft_F4_2), 4, 8);
}
#endif

#if R_SIMD_SIZE_S >= 4
void NS(RDftCore_StrideBatchDft_F4_4Test)() {
  NS(RDftCore_StrideBatchDftFunction_FTest)
  (NS(RDftCore_StrideBatchDft_F4_4), 4, 8);
}
#endif

#if R_SIMD_SIZE_S >= 8
void NS(RDftCore_StrideBatchDft_F4_8Test)() {
  NS(RDftCore_StrideBatchDftFunction_FTest)
  (NS(RDftCore_StrideBatchDft_F4_8), 4, 8);
}
#endif
