#include "PairTwiddleTransposeTest.h"

#include <jomfft/cdft/square/PairTwiddleTranspose.h>
#include <jomfft/math/Math.h>
#include <jomfft/util/ComplexTranspose.h>
#include <jomfft/util/random/ComplexTensor.h>

#include "../naive/TwiddleDft.h"

static void PairTwiddleTransposeTest__(bool backward, Long size,
                                       int num_threads) {
  CAPTURE_BOOL(backward);
  CAPTURE_LONG(size);
  CAPTURE_INT(num_threads);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  struct NS(ComplexFunction) *func = NS(CDftSquare_PairTwiddleTranspose_new)(
      data, backward, size, num_threads);
  struct NS(ComplexTensor) *x = NS(Random_ComplexTensor_new3)(size, 2, size);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_copy)(x);
  NS(ComplexFunction_evaluate)(func, y0->data, y0->data);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_copy)(x);
  for (Long i = 0; i < size; ++i) {
    NS(CDftNaive_TwiddleDft)(backward, 2, size,
                             NS(ComplexTensor_at3)(x, i, 0, 0),
                             NS(ComplexTensor_at3)(y1, i, 0, 0));
  }
  const Real sign = backward ? 1 : -1;
  const Real theta = (Real)(sign * 2 * M_PI) / (2 * size * size);
  for (Long i = 0; i < size; ++i) {
    for (Long j = 0; j < 2; ++j) {
      for (Long k = 0; k < size; ++k) {
        Complex z = NS(ComplexTensor_get3)(y1, i, j, k);
        Complex w = NS(Complex_fromArgument)(theta * (i * (size * j + k)));
        NS(ComplexTensor_set3)(y1, i, j, k, C_MUL(z, w));
      }
    }
  }
  for (int i = 0; i < 2; ++i) {
    NS(Complex_Transpose)
    (size, NS(ComplexTensor_at3)(y1, 0, i, 0), 2 * size);
  }
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
  NS(ComplexFunction_delete)(func);
  NS(DataFactory_delete)(data);
}

static void PairTwiddleTransposeTest_(bool backward, Long size) {
  PairTwiddleTransposeTest__(backward, size, 1);
}

void NS(CDftSquare_PairTwiddleTransposeTest)() {
  PairTwiddleTransposeTest_(false, 8);
  PairTwiddleTransposeTest_(true, 16);
  PairTwiddleTransposeTest_(true, 64);
#if USE_OPENMP
  PairTwiddleTransposeTest__(false, 64, 2);
  PairTwiddleTransposeTest__(true, 64, 2);
#endif
}
