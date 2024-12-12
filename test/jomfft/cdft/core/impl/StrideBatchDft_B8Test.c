#include "StrideBatchDft_B8Test.h"

#include <jomfft/cdft/core/impl/StrideBatchDft_B8.h>

#include "../StrideBatchDftFunctionTest.h"

#if C_SIMD_SIZE_S >= 1
void NS(CDftCore_StrideBatchDft_B8_1Test)() {
  NS(CDftCore_StrideBatchDftFunctionTest)
  (NS(CDftCore_StrideBatchDft_B8_1), true, 8, 8);
}
#endif

#if C_SIMD_SIZE_S >= 2
void NS(CDftCore_StrideBatchDft_B8_2Test)() {
  NS(CDftCore_StrideBatchDftFunctionTest)
  (NS(CDftCore_StrideBatchDft_B8_2), true, 8, 8);
}
#endif

#if C_SIMD_SIZE_S >= 4
void NS(CDftCore_StrideBatchDft_B8_4Test)() {
  NS(CDftCore_StrideBatchDftFunctionTest)
  (NS(CDftCore_StrideBatchDft_B8_4), true, 8, 8);
}
#endif
