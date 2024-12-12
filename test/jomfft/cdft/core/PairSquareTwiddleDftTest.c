#include "PairSquareTwiddleDftTest.h"

#include <jomfft/cdft/core/PairSquareTwiddleDft.h>
#include <jomfft/util/random/ComplexTensor.h>

#include "../naive/TwiddleDft.h"

static void CDftCore_PairSquareTwiddleDftTest_(bool backward, int dft_size) {
  CAPTURE_BOOL(backward);
  CAPTURE_INT(dft_size);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  struct NS(ComplexFunction) *func =
      NS(CDftCore_PairSquareTwiddleDft_new)(data, backward, dft_size);
  struct NS(ComplexTensor) *x =
      NS(Random_ComplexTensor_new3)(dft_size, 2, dft_size);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_zerosLike)(x);
  NS(ComplexFunction_evaluate)(func, x->data, y0->data);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(x);
  struct NS(ComplexTensor) *z = NS(ComplexTensor_zerosLike)(x);
  for (int i = 0; i < dft_size; ++i) {
    NS(CDftNaive_TwiddleDft)(backward, 2, dft_size,
                             NS(ComplexTensor_at3)(x, i, 0, 0),
                             NS(ComplexTensor_at3)(z, i, 0, 0));
  }
  NS(CDftNaive_TwiddleDft)(backward, dft_size, 2 * dft_size, z->data, y1->data);
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
  NS(ComplexFunction_delete)(func);
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
  NS(ComplexTensor_delete)(z);
  NS(DataFactory_delete)(data);
}

void NS(CDftCore_PairSquareTwiddleDftTest)() {
  CDftCore_PairSquareTwiddleDftTest_(false, 8);
  CDftCore_PairSquareTwiddleDftTest_(true, 16);
}
