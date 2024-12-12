#include "OuterBatchDftTest.h"

#include <jomfft/util/random/ComplexTensor.h>

#include "naive/BatchDft.h"

#define NS_(n) NS(CDft_##n)

void NS_(OuterBatchDftTest)(struct NS(ComplexFunction) *func, bool backward,
                            Long dft_size, Long batch_size, Long x_stride,
                            Long y_stride, bool inplace) {
  assert(func);
  assert(dft_size >= 2);
  assert(batch_size >= 1);
  assert(x_stride >= 1);
  assert(y_stride >= 1);
  CAPTURE_BOOL(backward);
  CAPTURE_LONG(dft_size);
  CAPTURE_LONG(batch_size);
  CAPTURE_LONG(x_stride);
  CAPTURE_LONG(y_stride);
  CAPTURE_BOOL(inplace);
  if (inplace && x_stride == y_stride) {
    const Long stride_0 = x_stride;
    const Long stride_1 = dft_size * stride_0;
    struct NS(ComplexTensor) *x =
        NS(Random_ComplexTensor_new3)(batch_size, dft_size, stride_0);
    struct NS(ComplexTensor) *y0 = NS(ComplexTensor_zerosLike)(x);
    struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(x);
    {
      Long sizes[] = {batch_size, dft_size, 1};
      struct NS(TensorShape) *shape = NS(TensorShape_new)(3, sizes);
      NS(ComplexTensor_CopyAs)(shape, x, y0);
      NS(TensorShape_delete)(shape);
    }
    NS(ComplexFunction_evaluate)(func, y0->data, y0->data);
    NS(CDftNaive_BatchDft)(backward, dft_size, batch_size, x->data, stride_0,
                           stride_1, y1->data, stride_0, stride_1);
    ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
    NS(ComplexTensor_delete)(x);
    NS(ComplexTensor_delete)(y0);
    NS(ComplexTensor_delete)(y1);
  } else {
    const Long x_stride_1 = dft_size * x_stride;
    const Long y_stride_1 = dft_size * y_stride;
    struct NS(ComplexTensor) *x =
        NS(Random_ComplexTensor_new3)(batch_size, dft_size, x_stride);
    struct NS(ComplexTensor) *x0 = NS(ComplexTensor_copy)(x);
    struct NS(ComplexTensor) *y0 =
        NS(ComplexTensor_new3)(batch_size, dft_size, y_stride);
    struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(y0);
    NS(ComplexFunction_evaluate)(func, x0->data, y0->data);
    NS(CDftNaive_BatchDft)(backward, dft_size, batch_size, x->data, x_stride,
                           x_stride_1, y1->data, y_stride, y_stride_1);
    ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
    NS(ComplexTensor_delete)(x);
    NS(ComplexTensor_delete)(x0);
    NS(ComplexTensor_delete)(y0);
    NS(ComplexTensor_delete)(y1);
  }
}
