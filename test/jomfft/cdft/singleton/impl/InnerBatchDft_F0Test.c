#include "InnerBatchDft_F0Test.h"

#include <jomfft/cdft/singleton/impl/InnerBatchDft_F0.h>

#include "../InnerBatchDftFunctionTest.h"

#if C_SIMD_SIZE >= 1
void NS(CDftSingleton_InnerBatchDft_F0_1Test)() {
  NS(CDftSingleton_InnerBatchDftFunctionTest)
  (NS(CDftSingleton_InnerBatchDft_F0_1), false, 8, 8);
}
#endif

#if C_SIMD_SIZE >= 2
void NS(CDftSingleton_InnerBatchDft_F0_2Test)() {
  NS(CDftSingleton_InnerBatchDftFunctionTest)
  (NS(CDftSingleton_InnerBatchDft_F0_2), false, 8, 8);
}
#endif

#if C_SIMD_SIZE >= 4
void NS(CDftSingleton_InnerBatchDft_F0_4Test)() {
  NS(CDftSingleton_InnerBatchDftFunctionTest)
  (NS(CDftSingleton_InnerBatchDft_F0_4), false, 8, 8);
}
#endif

#if C_SIMD_SIZE >= 8
void NS(CDftSingleton_InnerBatchDft_F0_8Test)() {
  NS(CDftSingleton_InnerBatchDftFunctionTest)
  (NS(CDftSingleton_InnerBatchDft_F0_8), false, 8, 8);
}
#endif
