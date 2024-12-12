#include "SquareSplitTwiddleDftTest.h"

#include <jomfft/cdft/core/SquareSplitTwiddleDft.h>
#include <jomfft/util/random/ComplexTensor.h>

#include "../naive/TwiddleDft.h"

static void CDftCore_SquareSplitTwiddleDftTest__(bool backward, int dft_size,
                                                 Long twiddle_size,
                                                 int num_threads) {
  CAPTURE_BOOL(backward);
  CAPTURE_INT(dft_size);
  CAPTURE_LONG(twiddle_size);
  CAPTURE_INT(num_threads);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  struct NS(ComplexFunction) *func = NS(CDftCore_SquareSplitTwiddleDft_new)(
      data, backward, dft_size, twiddle_size, num_threads);
  struct NS(ComplexTensor) *x =
      NS(Random_ComplexTensor_new3)(dft_size, twiddle_size, dft_size);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_zerosLike)(x);
  NS(ComplexFunction_evaluate)(func, x->data, y0->data);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(x);
  const Long stride = dft_size * twiddle_size;
  NS(CDftNaive_TwiddleDft)(backward, dft_size, stride, x->data, y1->data);
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
  NS(ComplexFunction_delete)(func);
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
  NS(DataFactory_delete)(data);
}

static void CDftCore_SquareSplitTwiddleDftTest_(bool backward, int dft_size,
                                                Long twiddle_size) {
  CDftCore_SquareSplitTwiddleDftTest__(backward, dft_size, twiddle_size, 1);
}

void NS(CDftCore_SquareSplitTwiddleDftTest)() {
  CDftCore_SquareSplitTwiddleDftTest_(false, 2, 2);
  CDftCore_SquareSplitTwiddleDftTest_(false, 2, 3);
  CDftCore_SquareSplitTwiddleDftTest_(true, 2, 4);
  CDftCore_SquareSplitTwiddleDftTest_(true, 2, 5);
  CDftCore_SquareSplitTwiddleDftTest_(false, 2, 6);
  CDftCore_SquareSplitTwiddleDftTest_(false, 2, 7);
  CDftCore_SquareSplitTwiddleDftTest_(false, 4, 2);
  CDftCore_SquareSplitTwiddleDftTest_(false, 4, 3);
  CDftCore_SquareSplitTwiddleDftTest_(true, 4, 4);
  CDftCore_SquareSplitTwiddleDftTest_(true, 4, 5);
  CDftCore_SquareSplitTwiddleDftTest_(false, 8, 4);
  CDftCore_SquareSplitTwiddleDftTest_(true, 8, 3);
#if USE_OPENMP
  CDftCore_SquareSplitTwiddleDftTest__(false, 8, 2, 2);
  CDftCore_SquareSplitTwiddleDftTest__(false, 4, 5, 2);
  CDftCore_SquareSplitTwiddleDftTest__(false, 2, 7, 2);
  CDftCore_SquareSplitTwiddleDftTest__(true, 8, 2, 2);
  CDftCore_SquareSplitTwiddleDftTest__(true, 4, 5, 2);
  CDftCore_SquareSplitTwiddleDftTest__(true, 2, 7, 2);
#endif
}
