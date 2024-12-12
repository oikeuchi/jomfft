#include "StrideBatchDft_B0Test.h"

#include <jomfft/cdft/singleton/impl/StrideBatchDft_B0.h>

#include "../StrideBatchDftFunctionTest.h"

#if C_SIMD_SIZE_S >= 1
void NS(CDftSingleton_StrideBatchDft_B0_1Test)() {
  NS(CDftSingleton_StrideBatchDftFunctionTest)
  (NS(CDftSingleton_StrideBatchDft_B0_1), true, 8, 8);
}
#endif

#if C_SIMD_SIZE_S >= 2
void NS(CDftSingleton_StrideBatchDft_B0_2Test)() {
  NS(CDftSingleton_StrideBatchDftFunctionTest)
  (NS(CDftSingleton_StrideBatchDft_B0_2), true, 8, 8);
}
#endif

#if C_SIMD_SIZE_S >= 4
void NS(CDftSingleton_StrideBatchDft_B0_4Test)() {
  NS(CDftSingleton_StrideBatchDftFunctionTest)
  (NS(CDftSingleton_StrideBatchDft_B0_4), true, 8, 8);
}
#endif
