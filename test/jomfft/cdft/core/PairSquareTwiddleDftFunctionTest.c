#include "PairSquareTwiddleDftFunctionTest.h"

#include <jomfft/data/TwiddleMatrix.h>
#include <jomfft/util/random/ComplexTensor.h>

#include "../naive/TwiddleDft.h"

void NS(CDftCore_PairSquareTwiddleDftFunctionTest)(
    NS(CDftCore_PairSquareTwiddleDft_fn) func, bool backward, int dft_size) {
  CAPTURE_BOOL(backward);
  CAPTURE_INT(dft_size);
  struct NS(ComplexTensor) *x =
      NS(Random_ComplexTensor_new3)(dft_size, 2, dft_size);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_zerosLike)(x);
  struct NS(TwiddleMatrixKey) w0_key = {2, dft_size, 2 * dft_size, 0, 0};
  struct NS(TwiddleMatrixKey) w1_key = {dft_size, 2 * dft_size,
                                        2 * dft_size * dft_size, 0, 0};
  Complex *w0 = NS(TwiddleMatrix_new)(w0_key);
  Complex *w1 = NS(TwiddleMatrix_new)(w1_key);
  func(w0, w1, x->data, y0->data);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(x);
  struct NS(ComplexTensor) *z = NS(ComplexTensor_zerosLike)(x);
  for (int i = 0; i < dft_size; ++i) {
    NS(CDftNaive_TwiddleDft)(backward, 2, dft_size,
                             NS(ComplexTensor_at3)(x, i, 0, 0),
                             NS(ComplexTensor_at3)(z, i, 0, 0));
  }
  NS(CDftNaive_TwiddleDft)(backward, dft_size, 2 * dft_size, z->data, y1->data);
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
  NS(ComplexTensor_delete)(z);
  NS(TwiddleMatrix_delete)(w0);
  NS(TwiddleMatrix_delete)(w1);
}
