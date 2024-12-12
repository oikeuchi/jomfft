#include "BatchMultiply_FTest.h"

#include <jomfft/cdft/core/impl/BatchMultiply_F.h>

#include "../BatchMultiplyFunctionTest.h"

#if C_SIMD_SIZE_S >= 1
void NS(CDftCore_BatchMultiply_F_1Test)() {
  NS(CDftCore_BatchMultiplyFunctionTest)
  (NS(CDftCore_BatchMultiply_F_1), false, 8);
}
#endif

#if C_SIMD_SIZE_S >= 2
void NS(CDftCore_BatchMultiply_F_2Test)() {
  NS(CDftCore_BatchMultiplyFunctionTest)
  (NS(CDftCore_BatchMultiply_F_2), false, 8);
}
#endif

#if C_SIMD_SIZE_S >= 4
void NS(CDftCore_BatchMultiply_F_4Test)() {
  NS(CDftCore_BatchMultiplyFunctionTest)
  (NS(CDftCore_BatchMultiply_F_4), false, 8);
}
#endif
