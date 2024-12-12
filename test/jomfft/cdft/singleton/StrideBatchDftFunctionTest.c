#include "StrideBatchDftFunctionTest.h"

#include <jomfft/data/TwiddleMatrix.h>
#include <jomfft/util/random/ComplexTensor.h>

#include "../naive/BatchDft.h"

void NS(CDftSingleton_StrideBatchDftFunctionTest)(
    NS(CDftSingleton_StrideBatchDft_fn) func, bool backward, int dft_size,
    Long batch_size) {
  CAPTURE_BOOL(backward);
  CAPTURE_INT(dft_size);
  CAPTURE_LONG(batch_size);
  const Long stride_0 = 2 * batch_size;
  const Long stride_1 = 2;
  struct NS(ComplexTensor) *x =
      NS(Random_ComplexTensor_new2)(dft_size, stride_0);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_zerosLike)(x);
  const int m = (dft_size - 1) / 2;
  struct NS(TwiddleMatrixKey) w_key = {m, m, dft_size, 1, 1};
  Complex *w = NS(TwiddleMatrix_new)(w_key);
  func(dft_size, w, batch_size, x->data, stride_0, stride_1, y0->data, stride_0,
       stride_1);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(x);
  NS(CDftNaive_BatchDft)(backward, dft_size, batch_size, x->data, stride_0,
                         stride_1, y1->data, stride_0, stride_1);
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
  NS(TwiddleMatrix_delete)(w);
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
}
