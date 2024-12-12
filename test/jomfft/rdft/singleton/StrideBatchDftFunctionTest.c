#include "StrideBatchDftFunctionTest.h"

#include <jomfft/data/TwiddleMatrix.h>
#include <jomfft/util/random/ConjugateEvenTensor.h>
#include <jomfft/util/random/RealTensor.h>

#include "../naive/BatchDft.h"

void NS(RDftSingleton_StrideBatchDftFunction_FTest)(
    NS(RDftSingleton_StrideBatchDft_F_fn) func, int dft_size, Long batch_size) {
  CAPTURE_INT(dft_size);
  CAPTURE_LONG(batch_size);
  const Long x_stride_0 = 1;
  const Long x_stride_1 = dft_size;
  const Long y_stride_0 = 1;
  const Long y_stride_1 = CE_SIZE(dft_size);
  struct NS(RealTensor) *x = NS(Random_RealTensor_new2)(batch_size, dft_size);
  struct NS(ComplexTensor) *y0 =
      NS(ComplexTensor_new2)(batch_size, CE_SIZE(dft_size));
  const int m = (dft_size - 1) / 2;
  struct NS(TwiddleMatrixKey) w_key = {m, m, dft_size, 1, 1};
  Complex *w = NS(TwiddleMatrix_new)(w_key);
  func(dft_size, w, batch_size, x->data, x_stride_0, x_stride_1, y0->data,
       y_stride_0, y_stride_1);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(y0);
  NS(RDftNaive_BatchDft_F)(dft_size, batch_size, x->data, x_stride_0,
                           x_stride_1, y1->data, y_stride_0, y_stride_1);
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
  NS(TwiddleMatrix_delete)(w);
  NS(RealTensor_delete)(x);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
}

void NS(RDftSingleton_StrideBatchDftFunction_BTest)(
    NS(RDftSingleton_StrideBatchDft_B_fn) func, int dft_size, Long batch_size) {
  CAPTURE_INT(dft_size);
  CAPTURE_LONG(batch_size);
  const Long x_stride_0 = 1;
  const Long x_stride_1 = CE_SIZE(dft_size);
  const Long y_stride_0 = 1;
  const Long y_stride_1 = dft_size;
  struct NS(ComplexTensor) *x =
      NS(Random_ConjugateEvenTensor_new2)(batch_size, dft_size);
  struct NS(RealTensor) *y0 = NS(RealTensor_new2)(batch_size, dft_size);
  const int m = (dft_size - 1) / 2;
  struct NS(TwiddleMatrixKey) w_key = {m, m, dft_size, 1, 1};
  Complex *w = NS(TwiddleMatrix_new)(w_key);
  func(dft_size, w, batch_size, x->data, x_stride_0, x_stride_1, y0->data,
       y_stride_0, y_stride_1);
  struct NS(RealTensor) *y1 = NS(RealTensor_zerosLike)(y0);
  NS(RDftNaive_BatchDft_B)(dft_size, batch_size, x->data, x_stride_0,
                           x_stride_1, y1->data, y_stride_0, y_stride_1);
  ASSERT_REALTENSOR_AEQ(y0, y1);
  NS(TwiddleMatrix_delete)(w);
  NS(ComplexTensor_delete)(x);
  NS(RealTensor_delete)(y0);
  NS(RealTensor_delete)(y1);
}
