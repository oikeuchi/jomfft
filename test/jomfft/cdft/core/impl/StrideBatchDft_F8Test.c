#include "StrideBatchDft_F8Test.h"

#include <jomfft/cdft/core/impl/StrideBatchDft_F8.h>

#include "../StrideBatchDftFunctionTest.h"

#if C_SIMD_SIZE_S >= 1
void NS(CDftCore_StrideBatchDft_F8_1Test)() {
  NS(CDftCore_StrideBatchDftFunctionTest)
  (NS(CDftCore_StrideBatchDft_F8_1), false, 8, 8);
}
#endif

#if C_SIMD_SIZE_S >= 2
void NS(CDftCore_StrideBatchDft_F8_2Test)() {
  NS(CDftCore_StrideBatchDftFunctionTest)
  (NS(CDftCore_StrideBatchDft_F8_2), false, 8, 8);
}
#endif

#if C_SIMD_SIZE_S >= 4
void NS(CDftCore_StrideBatchDft_F8_4Test)() {
  NS(CDftCore_StrideBatchDftFunctionTest)
  (NS(CDftCore_StrideBatchDft_F8_4), false, 8, 8);
}
#endif
