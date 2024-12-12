#include "TwiddleDftFunctionTest.h"

#include <jomfft/data/TwiddleMatrix.h>
#include <jomfft/util/random/ComplexTensor.h>

#include "../naive/TwiddleDft.h"

void NS(CDftCore_TwiddleDftFunctionTest)(NS(CDftCore_TwiddleDft_fn) func,
                                         bool backward, int dft_size,
                                         Long twiddle_size) {
  CAPTURE_BOOL(backward);
  CAPTURE_INT(dft_size);
  CAPTURE_LONG(twiddle_size);
  struct NS(ComplexTensor) *x =
      NS(Random_ComplexTensor_new2)(dft_size, twiddle_size);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_zerosLike)(x);
  struct NS(TwiddleMatrixKey) w_key = {dft_size - 1, twiddle_size,
                                       dft_size * twiddle_size, 1, 0};
  Complex *w = NS(TwiddleMatrix_new)(w_key);
  func(twiddle_size, w, x->data, y0->data, twiddle_size);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(x);
  NS(CDftNaive_TwiddleDft)(backward, dft_size, twiddle_size, x->data, y1->data);
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
  NS(TwiddleMatrix_delete)(w);
}
