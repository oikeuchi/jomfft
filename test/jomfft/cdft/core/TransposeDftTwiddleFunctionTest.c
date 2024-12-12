#include "TransposeDftTwiddleFunctionTest.h"

#include <jomfft/data/TwiddleMatrix.h>
#include <jomfft/util/random/ComplexTensor.h>

#include "../naive/BatchDft.h"

void NS(CDftCore_TransposeDftTwiddleFunctionTest)(
    NS(CDftCore_TransposeDftTwiddle_fn) func, bool backward, int dft_size,
    Long twiddle_size) {
  CAPTURE_BOOL(backward);
  CAPTURE_INT(dft_size);
  CAPTURE_LONG(twiddle_size);
  const Long stride = dft_size * twiddle_size;
  struct NS(ComplexTensor) *x =
      NS(Random_ComplexTensor_new3)(dft_size, twiddle_size, dft_size);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_zerosLike)(x);
  struct NS(TwiddleMatrixKey) w_key = {twiddle_size, dft_size,
                                       twiddle_size * dft_size, 0, 0};
  Complex *w = NS(TwiddleMatrix_new)(w_key);
  func(stride, w, x->data, stride, 1, y0->data, stride);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(x);
  for (Long j = 0; j < twiddle_size; ++j) {
    NS(CDftNaive_BatchDft)(backward, dft_size, dft_size,
                           NS(ComplexTensor_at3)(x, 0, j, 0), stride, 1,
                           NS(ComplexTensor_at3)(y1, 0, j, 0), 1, stride);
  }
  for (Long i = 0; i < dft_size; ++i) {
    const Complex *w0 = w;
    Complex *y1_ = NS(ComplexTensor_at3)(y1, i, 0, 0);
    if (backward) {
      for (Long k = 0; k < stride; ++k) {
        y1_[k] = C_MUL(y1_[k], C_CONJ(w0[k]));
      }
    } else {
      for (Long k = 0; k < stride; ++k) {
        y1_[k] = C_MUL(y1_[k], w0[k]);
      }
    }
  }
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
  NS(TwiddleMatrix_delete)(w);
}
