#include "StrideBatchDft_F0Test.h"

#include <jomfft/rdft/singleton/impl/StrideBatchDft_F0.h>

#include "../StrideBatchDftFunctionTest.h"

#if R_SIMD_SIZE_S >= 1
void NS(RDftSingleton_StrideBatchDft_F0_1Test)() {
  NS(RDftSingleton_StrideBatchDftFunction_FTest)
  (NS(RDftSingleton_StrideBatchDft_F0_1), 8, 8);
}
#endif

#if R_SIMD_SIZE_S >= 2
void NS(RDftSingleton_StrideBatchDft_F0_2Test)() {
  NS(RDftSingleton_StrideBatchDftFunction_FTest)
  (NS(RDftSingleton_StrideBatchDft_F0_2), 8, 8);
}
#endif

#if R_SIMD_SIZE_S >= 4
void NS(RDftSingleton_StrideBatchDft_F0_4Test)() {
  NS(RDftSingleton_StrideBatchDftFunction_FTest)
  (NS(RDftSingleton_StrideBatchDft_F0_4), 8, 8);
}
#endif

#if R_SIMD_SIZE_S >= 8
void NS(RDftSingleton_StrideBatchDft_F0_8Test)() {
  NS(RDftSingleton_StrideBatchDftFunction_FTest)
  (NS(RDftSingleton_StrideBatchDft_F0_4), 8, 8);
}
#endif
