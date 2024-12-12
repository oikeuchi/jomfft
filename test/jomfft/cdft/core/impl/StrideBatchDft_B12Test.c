#include "StrideBatchDft_B12Test.h"

#include <jomfft/cdft/core/impl/StrideBatchDft_B12.h>

#include "../StrideBatchDftFunctionTest.h"

#if C_SIMD_SIZE_S >= 1
void NS(CDftCore_StrideBatchDft_B12_1Test)() {
  NS(CDftCore_StrideBatchDftFunctionTest)
  (NS(CDftCore_StrideBatchDft_B12_1), true, 12, 8);
}
#endif

#if C_SIMD_SIZE_S >= 2
void NS(CDftCore_StrideBatchDft_B12_2Test)() {
  NS(CDftCore_StrideBatchDftFunctionTest)
  (NS(CDftCore_StrideBatchDft_B12_2), true, 12, 8);
}
#endif

#if C_SIMD_SIZE_S >= 4
void NS(CDftCore_StrideBatchDft_B12_4Test)() {
  NS(CDftCore_StrideBatchDftFunctionTest)
  (NS(CDftCore_StrideBatchDft_B12_4), true, 12, 8);
}
#endif
