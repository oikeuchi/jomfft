#include "TwiddleTransposeTest.h"

#include <jomfft/cdft/square/TwiddleTranspose.h>
#include <jomfft/math/Math.h>
#include <jomfft/util/ComplexTranspose.h>
#include <jomfft/util/random/ComplexTensor.h>

static void TwiddleTransposeTest__(bool backward, Long size, int num_threads) {
  CAPTURE_BOOL(backward);
  CAPTURE_LONG(size);
  CAPTURE_INT(num_threads);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  struct NS(ComplexFunction) *func =
      NS(CDftSquare_TwiddleTranspose_new)(data, backward, size, num_threads);
  struct NS(ComplexTensor) *x = NS(Random_ComplexTensor_new2)(size, size);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_copy)(x);
  NS(ComplexFunction_evaluate)(func, y0->data, y0->data);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_copy)(x);
  const Real sign = backward ? 1 : -1;
  const Real theta = (Real)(sign * 2 * M_PI) / (size * size);
  for (Long i = 0; i < size; ++i) {
    for (Long j = 0; j < size; ++j) {
      Complex z = NS(ComplexTensor_get2)(y1, i, j);
      Complex w = NS(Complex_fromArgument)(theta * (i * j));
      NS(ComplexTensor_set2)(y1, i, j, C_MUL(z, w));
    }
  }
  NS(Complex_Transpose)(size, y1->data, size);
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
  NS(ComplexFunction_delete)(func);
  NS(DataFactory_delete)(data);
}

static void TwiddleTransposeTest_(bool backward, Long size) {
  TwiddleTransposeTest__(backward, size, 1);
}

void NS(CDftSquare_TwiddleTransposeTest)() {
  TwiddleTransposeTest_(false, 8);
  TwiddleTransposeTest_(true, 16);
  TwiddleTransposeTest_(true, 64);
#if USE_OPENMP
  TwiddleTransposeTest__(false, 64, 2);
  TwiddleTransposeTest__(true, 64, 2);
#endif
}
