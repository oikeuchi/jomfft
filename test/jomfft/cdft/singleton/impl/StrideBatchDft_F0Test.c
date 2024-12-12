#include "StrideBatchDft_F0Test.h"

#include <jomfft/cdft/singleton/impl/StrideBatchDft_F0.h>

#include "../StrideBatchDftFunctionTest.h"

#if C_SIMD_SIZE_S >= 1
void NS(CDftSingleton_StrideBatchDft_F0_1Test)() {
  NS(CDftSingleton_StrideBatchDftFunctionTest)
  (NS(CDftSingleton_StrideBatchDft_F0_1), false, 8, 8);
}
#endif

#if C_SIMD_SIZE_S >= 2
void NS(CDftSingleton_StrideBatchDft_F0_2Test)() {
  NS(CDftSingleton_StrideBatchDftFunctionTest)
  (NS(CDftSingleton_StrideBatchDft_F0_2), false, 8, 8);
}
#endif

#if C_SIMD_SIZE_S >= 4
void NS(CDftSingleton_StrideBatchDft_F0_4Test)() {
  NS(CDftSingleton_StrideBatchDftFunctionTest)
  (NS(CDftSingleton_StrideBatchDft_F0_4), false, 8, 8);
}
#endif
