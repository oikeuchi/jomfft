#include "InnerBatchDftTest.h"

#include <jomfft/util/random/ComplexTensor.h>

#include "naive/BatchDft.h"

#define NS_(n) NS(CDft_##n)

void NS_(InnerBatchDftTest)(struct NS(ComplexFunction) *func, bool backward,
                            Long dft_size, Long batch_size, bool inplace) {
  assert(func);
  assert(dft_size >= 2);
  assert(batch_size >= 1);
  CAPTURE_BOOL(backward);
  CAPTURE_LONG(dft_size);
  CAPTURE_LONG(batch_size);
  CAPTURE_BOOL(inplace);
  const Long stride = batch_size;
  struct NS(ComplexTensor) *x =
      NS(Random_ComplexTensor_new2)(dft_size, batch_size);
  struct NS(ComplexTensor) *x0 = NS(ComplexTensor_copy)(x);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_zerosLike)(x);
  if (inplace) {
    NS(ComplexTensor_CopyAs)(x0->shape, x0, y0);
    NS(ComplexFunction_evaluate)(func, y0->data, y0->data);
  } else {
    NS(ComplexFunction_evaluate)(func, x0->data, y0->data);
  }
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(x);
  NS(CDftNaive_BatchDft)(backward, dft_size, batch_size, x->data, stride, 1,
                         y1->data, stride, 1);
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(x0);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
}
