#include "TransposeDftTwiddleTest.h"

#include <jomfft/cdft/core/TransposeDftTwiddle.h>
#include <jomfft/data/TwiddleMatrix.h>
#include <jomfft/util/random/ComplexTensor.h>

#include "../naive/BatchDft.h"

static void CDftCore_TransposeDftTwiddleTest__(bool backward, int dft_size,
                                               Long twiddle_size,
                                               int num_threads) {
  CAPTURE_BOOL(backward);
  CAPTURE_INT(dft_size);
  CAPTURE_LONG(twiddle_size);
  CAPTURE_INT(num_threads);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  struct NS(ComplexFunction) *func = NS(CDftCore_TransposeDftTwiddle_new)(
      data, backward, dft_size, twiddle_size, 1, num_threads);
  struct NS(ComplexTensor) *x =
      NS(Random_ComplexTensor_new3)(dft_size, twiddle_size, dft_size);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_zerosLike)(x);
  NS(ComplexFunction_evaluate)(func, x->data, y0->data);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(x);
  const Long stride = dft_size * twiddle_size;
  for (Long j = 0; j < twiddle_size; ++j) {
    NS(CDftNaive_BatchDft)(backward, dft_size, dft_size,
                           NS(ComplexTensor_at3)(x, 0, j, 0), stride, 1,
                           NS(ComplexTensor_at3)(y1, 0, j, 0), 1, stride);
  }
  const Complex *w = NS(DataFactory_twiddleMatrix)(
      data, twiddle_size, dft_size, twiddle_size * dft_size, 0, 0);
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
  NS(ComplexFunction_delete)(func);
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
  NS(DataFactory_delete)(data);
}

static void CDftCore_TransposeDftTwiddleTest_(bool backward, int dft_size,
                                              Long twiddle_size) {
  CDftCore_TransposeDftTwiddleTest__(backward, dft_size, twiddle_size, 1);
}

void NS(CDftCore_TransposeDftTwiddleTest)() {
  CDftCore_TransposeDftTwiddleTest_(false, 2, 2);
  CDftCore_TransposeDftTwiddleTest_(false, 2, 3);
  CDftCore_TransposeDftTwiddleTest_(true, 2, 4);
  CDftCore_TransposeDftTwiddleTest_(true, 2, 5);
  CDftCore_TransposeDftTwiddleTest_(false, 2, 6);
  CDftCore_TransposeDftTwiddleTest_(false, 2, 7);
  CDftCore_TransposeDftTwiddleTest_(false, 4, 2);
  CDftCore_TransposeDftTwiddleTest_(false, 4, 3);
  CDftCore_TransposeDftTwiddleTest_(true, 4, 4);
  CDftCore_TransposeDftTwiddleTest_(true, 4, 5);
  CDftCore_TransposeDftTwiddleTest_(false, 8, 4);
  CDftCore_TransposeDftTwiddleTest_(true, 8, 3);
#if USE_OPENMP
  CDftCore_TransposeDftTwiddleTest__(false, 2, 4, 2);
  CDftCore_TransposeDftTwiddleTest__(true, 2, 9, 2);
  CDftCore_TransposeDftTwiddleTest__(false, 4, 5, 2);
  CDftCore_TransposeDftTwiddleTest__(true, 4, 8, 2);
  CDftCore_TransposeDftTwiddleTest__(false, 8, 4, 2);
  CDftCore_TransposeDftTwiddleTest__(true, 8, 3, 2);
#endif
}
