#include "StrideBatchDft_F6Test.h"

#include <jomfft/cdft/core/impl/StrideBatchDft_F6.h>

#include "../StrideBatchDftFunctionTest.h"

#if C_SIMD_SIZE_S >= 1
void NS(CDftCore_StrideBatchDft_F6_1Test)() {
  NS(CDftCore_StrideBatchDftFunctionTest)
  (NS(CDftCore_StrideBatchDft_F6_1), false, 6, 8);
}
#endif

#if C_SIMD_SIZE_S >= 2
void NS(CDftCore_StrideBatchDft_F6_2Test)() {
  NS(CDftCore_StrideBatchDftFunctionTest)
  (NS(CDftCore_StrideBatchDft_F6_2), false, 6, 8);
}
#endif

#if C_SIMD_SIZE_S >= 4
void NS(CDftCore_StrideBatchDft_F6_4Test)() {
  NS(CDftCore_StrideBatchDftFunctionTest)
  (NS(CDftCore_StrideBatchDft_F6_4), false, 6, 8);
}
#endif
