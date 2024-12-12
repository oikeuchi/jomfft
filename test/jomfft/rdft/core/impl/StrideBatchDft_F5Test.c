#include "StrideBatchDft_F5Test.h"

#include <jomfft/rdft/core/impl/StrideBatchDft_F5.h>

#include "../StrideBatchDftFunctionTest.h"

#if R_SIMD_SIZE_S >= 1
void NS(RDftCore_StrideBatchDft_F5_1Test)() {
  NS(RDftCore_StrideBatchDftFunction_FTest)
  (NS(RDftCore_StrideBatchDft_F5_1), 5, 8);
}
#endif

#if R_SIMD_SIZE_S >= 2
void NS(RDftCore_StrideBatchDft_F5_2Test)() {
  NS(RDftCore_StrideBatchDftFunction_FTest)
  (NS(RDftCore_StrideBatchDft_F5_2), 5, 8);
}
#endif

#if R_SIMD_SIZE_S >= 4
void NS(RDftCore_StrideBatchDft_F5_4Test)() {
  NS(RDftCore_StrideBatchDftFunction_FTest)
  (NS(RDftCore_StrideBatchDft_F5_4), 5, 8);
}
#endif

#if R_SIMD_SIZE_S >= 8
void NS(RDftCore_StrideBatchDft_F5_8Test)() {
  NS(RDftCore_StrideBatchDftFunction_FTest)
  (NS(RDftCore_StrideBatchDft_F5_8), 5, 8);
}
#endif
