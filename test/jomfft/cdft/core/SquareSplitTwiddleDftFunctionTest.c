#include "SquareSplitTwiddleDftFunctionTest.h"

#include <jomfft/data/TwiddleMatrix.h>
#include <jomfft/util/random/ComplexTensor.h>

#include "../naive/TwiddleDft.h"

void NS(CDftCore_SquareSplitTwiddleDftFunctionTest_)(
    NS(CDftCore_SquareSplitTwiddleDft_fn) func, bool backward, int dft_size,
    Long twiddle_size, int vector_size) {
  CAPTURE_BOOL(backward);
  CAPTURE_INT(dft_size);
  CAPTURE_LONG(twiddle_size);
  CAPTURE_INT(vector_size);
  const Long stride = dft_size * twiddle_size;
  struct NS(ComplexTensor) *x =
      NS(Random_ComplexTensor_new3)(dft_size, twiddle_size, dft_size);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_zerosLike)(x);
  struct NS(TwiddleMatrixKey) v_key = {twiddle_size, dft_size,
                                       twiddle_size * dft_size, 0, 0};
  Complex *v = NS(TwiddleMatrix_new)(v_key);
  int w_stride = vector_size > dft_size ? vector_size : dft_size;
  struct NS(TwiddleMatrixKey) w_key = {
      dft_size - 1, w_stride, twiddle_size * dft_size * dft_size, 1, 0};
  Complex *w = NS(TwiddleMatrix_new)(w_key);
  func(stride, v, w, w_stride, x->data, y0->data, stride);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(x);
  NS(CDftNaive_TwiddleDft)(backward, dft_size, stride, x->data, y1->data);
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
  NS(TwiddleMatrix_delete)(v);
  NS(TwiddleMatrix_delete)(w);
}

void NS(CDftCore_SquareSplitTwiddleDftFunctionTest)(
    NS(CDftCore_SquareSplitTwiddleDft_fn) func, bool backward, int dft_size,
    Long twiddle_size) {
  NS(CDftCore_SquareSplitTwiddleDftFunctionTest_)
  (func, backward, dft_size, twiddle_size, 1);
}
