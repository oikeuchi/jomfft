#include "InnerBatchDft_B0Test.h"

#include <jomfft/cdft/singleton/impl/InnerBatchDft_B0.h>

#include "../InnerBatchDftFunctionTest.h"

#if C_SIMD_SIZE >= 1
void NS(CDftSingleton_InnerBatchDft_B0_1Test)() {
  NS(CDftSingleton_InnerBatchDftFunctionTest)
  (NS(CDftSingleton_InnerBatchDft_B0_1), true, 8, 8);
}
#endif

#if C_SIMD_SIZE >= 2
void NS(CDftSingleton_InnerBatchDft_B0_2Test)() {
  NS(CDftSingleton_InnerBatchDftFunctionTest)
  (NS(CDftSingleton_InnerBatchDft_B0_2), true, 8, 8);
}
#endif

#if C_SIMD_SIZE >= 4
void NS(CDftSingleton_InnerBatchDft_B0_4Test)() {
  NS(CDftSingleton_InnerBatchDftFunctionTest)
  (NS(CDftSingleton_InnerBatchDft_B0_4), true, 8, 8);
}
#endif

#if C_SIMD_SIZE >= 8
void NS(CDftSingleton_InnerBatchDft_B0_8Test)() {
  NS(CDftSingleton_InnerBatchDftFunctionTest)
  (NS(CDftSingleton_InnerBatchDft_B0_8), true, 8, 8);
}
#endif
