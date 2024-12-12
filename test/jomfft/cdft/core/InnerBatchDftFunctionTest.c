#include "InnerBatchDftFunctionTest.h"

#include <jomfft/util/random/ComplexTensor.h>

#include "../naive/BatchDft.h"

void NS(CDftCore_InnerBatchDftFunctionTest)(NS(CDftCore_InnerBatchDft_fn) func,
                                            bool backward, int dft_size,
                                            Long batch_size) {
  CAPTURE_BOOL(backward);
  CAPTURE_INT(dft_size);
  CAPTURE_LONG(batch_size);
  struct NS(ComplexTensor) *x =
      NS(Random_ComplexTensor_new2)(dft_size, batch_size);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_zerosLike)(x);
  func(batch_size, x->data, batch_size, y0->data, batch_size);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(x);
  NS(CDftNaive_BatchDft)(backward, dft_size, batch_size, x->data, batch_size, 1,
                         y1->data, batch_size, 1);
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
}
