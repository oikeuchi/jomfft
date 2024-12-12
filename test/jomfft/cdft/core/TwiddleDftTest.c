#include "TwiddleDftTest.h"

#include <jomfft/cdft/core/TwiddleDft.h>
#include <jomfft/util/random/ComplexTensor.h>

#include "../naive/TwiddleDft.h"

static void CDftCore_TwiddleDftTest__(bool backward, int dft_size,
                                      Long twiddle_size, int num_threads) {
  assert(twiddle_size >= 1);
  CAPTURE_BOOL(backward);
  CAPTURE_INT(dft_size);
  CAPTURE_LONG(twiddle_size);
  CAPTURE_INT(num_threads);

  struct NS(DataFactory) *data = NS(DataFactory_new)();
  struct NS(ComplexFunction) *func =
      NS(CDftCore_TwiddleDft_new)(data, backward, dft_size, twiddle_size);
  struct NS(ComplexTensor) *x =
      NS(Random_ComplexTensor_new2)(dft_size, twiddle_size);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_zerosLike)(x);
  NS(ComplexFunction_evaluate)(func, x->data, y0->data);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(x);
  NS(CDftNaive_TwiddleDft)(backward, dft_size, twiddle_size, x->data, y1->data);
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
  NS(ComplexFunction_delete)(func);
  NS(DataFactory_delete)(data);
}

static void CDftCore_TwiddleDftTest_(bool backward, int dft_size,
                                     Long twiddle_size) {
  CDftCore_TwiddleDftTest__(backward, dft_size, twiddle_size, 1);
}

void NS(CDftCore_TwiddleDftTest)() {
  CDftCore_TwiddleDftTest_(false, 2, 1);
  CDftCore_TwiddleDftTest_(false, 4, 2);
  CDftCore_TwiddleDftTest_(true, 8, 3);
  CDftCore_TwiddleDftTest_(true, 16, 4);
  CDftCore_TwiddleDftTest_(false, 2, 5);
#if USE_OPENMP
  CDftCore_TwiddleDftTest__(false, 4, 4, 2);
  CDftCore_TwiddleDftTest__(true, 4, 5, 2);
  CDftCore_TwiddleDftTest__(false, 2, 4, 2);
  CDftCore_TwiddleDftTest__(true, 2, 5, 2);
#endif
}
