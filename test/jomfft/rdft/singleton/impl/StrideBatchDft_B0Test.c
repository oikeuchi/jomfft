#include "StrideBatchDft_B0Test.h"

#include <jomfft/rdft/singleton/impl/StrideBatchDft_B0.h>

#include "../StrideBatchDftFunctionTest.h"

#if R_SIMD_SIZE_S >= 1
void NS(RDftSingleton_StrideBatchDft_B0_1Test)() {
  NS(RDftSingleton_StrideBatchDftFunction_BTest)
  (NS(RDftSingleton_StrideBatchDft_B0_1), 8, 8);
}
#endif

#if R_SIMD_SIZE_S >= 2
void NS(RDftSingleton_StrideBatchDft_B0_2Test)() {
  NS(RDftSingleton_StrideBatchDftFunction_BTest)
  (NS(RDftSingleton_StrideBatchDft_B0_2), 8, 8);
}
#endif

#if R_SIMD_SIZE_S >= 4
void NS(RDftSingleton_StrideBatchDft_B0_4Test)() {
  NS(RDftSingleton_StrideBatchDftFunction_BTest)
  (NS(RDftSingleton_StrideBatchDft_B0_4), 8, 8);
}
#endif

#if R_SIMD_SIZE_S >= 8
void NS(RDftSingleton_StrideBatchDft_B0_8Test)() {
  NS(RDftSingleton_StrideBatchDftFunction_BTest)
  (NS(RDftSingleton_StrideBatchDft_B0_4), 8, 8);
}
#endif
