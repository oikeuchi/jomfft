#include "BatchMultiply_BTest.h"

#include <jomfft/cdft/core/impl/BatchMultiply_B.h>

#include "../BatchMultiplyFunctionTest.h"

#if C_SIMD_SIZE_S >= 1
void NS(CDftCore_BatchMultiply_B_1Test)() {
  NS(CDftCore_BatchMultiplyFunctionTest)
  (NS(CDftCore_BatchMultiply_B_1), true, 8);
}
#endif

#if C_SIMD_SIZE_S >= 2
void NS(CDftCore_BatchMultiply_B_2Test)() {
  NS(CDftCore_BatchMultiplyFunctionTest)
  (NS(CDftCore_BatchMultiply_B_2), true, 8);
}
#endif

#if C_SIMD_SIZE_S >= 4
void NS(CDftCore_BatchMultiply_B_4Test)() {
  NS(CDftCore_BatchMultiplyFunctionTest)
  (NS(CDftCore_BatchMultiply_B_4), true, 8);
}
#endif
