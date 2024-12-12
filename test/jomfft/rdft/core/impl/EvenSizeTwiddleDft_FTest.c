#include "EvenSizeTwiddleDft_FTest.h"

#include <jomfft/rdft/core/impl/EvenSizeTwiddleDft_F.h>

#include "../EvenSizeTwiddleDftFunctionTest.h"

#if C_SIMD_SIZE >= 1
void NS(RDftCore_EvenSizeTwiddleDft_F_1Test)() {
  NS(RDftCore_EvenSizeTwiddleDftFunction_FTest)
  (NS(RDftCore_EvenSizeTwiddleDft_F_1), 8);
}
#endif

#if C_SIMD_SIZE >= 2
void NS(RDftCore_EvenSizeTwiddleDft_F_2Test)() {
  NS(RDftCore_EvenSizeTwiddleDftFunction_FTest)
  (NS(RDftCore_EvenSizeTwiddleDft_F_2), 8);
}
#endif

#if C_SIMD_SIZE >= 4
void NS(RDftCore_EvenSizeTwiddleDft_F_4Test)() {
  NS(RDftCore_EvenSizeTwiddleDftFunction_FTest)
  (NS(RDftCore_EvenSizeTwiddleDft_F_4), 8);
}
#endif

#if C_SIMD_SIZE >= 8
void NS(RDftCore_EvenSizeTwiddleDft_F_8Test)() {
  NS(RDftCore_EvenSizeTwiddleDftFunction_FTest)
  (NS(RDftCore_EvenSizeTwiddleDft_F_8), 8);
}
#endif
