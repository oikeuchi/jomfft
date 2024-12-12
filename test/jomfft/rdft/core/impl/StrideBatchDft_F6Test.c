#include "StrideBatchDft_F6Test.h"

#include <jomfft/rdft/core/impl/StrideBatchDft_F6.h>

#include "../StrideBatchDftFunctionTest.h"

#if R_SIMD_SIZE_S >= 1
void NS(RDftCore_StrideBatchDft_F6_1Test)() {
  NS(RDftCore_StrideBatchDftFunction_FTest)
  (NS(RDftCore_StrideBatchDft_F6_1), 6, 8);
}
#endif

#if R_SIMD_SIZE_S >= 2
void NS(RDftCore_StrideBatchDft_F6_2Test)() {
  NS(RDftCore_StrideBatchDftFunction_FTest)
  (NS(RDftCore_StrideBatchDft_F6_2), 6, 8);
}
#endif

#if R_SIMD_SIZE_S >= 4
void NS(RDftCore_StrideBatchDft_F6_4Test)() {
  NS(RDftCore_StrideBatchDftFunction_FTest)
  (NS(RDftCore_StrideBatchDft_F6_4), 6, 8);
}
#endif

#if R_SIMD_SIZE_S >= 8
void NS(RDftCore_StrideBatchDft_F6_8Test)() {
  NS(RDftCore_StrideBatchDftFunction_FTest)
  (NS(RDftCore_StrideBatchDft_F6_8), 6, 8);
}
#endif
