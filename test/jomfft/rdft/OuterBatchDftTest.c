#include "OuterBatchDftTest.h"

#include <jomfft/util/random/ConjugateEvenTensor.h>
#include <jomfft/util/random/RealTensor.h>

#include "naive/BatchDft.h"

#define NS_(n) NS(RDft_##n)

void NS_(OuterBatchDft_FTest)(struct NS(RealToComplexFunction) *func,
                              Long dft_size, Long batch_size, Long x_stride,
                              Long y_stride, bool inplace) {
  assert(func);
  assert(dft_size >= 2);
  assert(batch_size >= 1);
  assert(x_stride >= 1);
  assert(y_stride >= 1);
  CAPTURE_LONG(dft_size);
  CAPTURE_LONG(batch_size);
  CAPTURE_LONG(x_stride);
  CAPTURE_LONG(y_stride);
  CAPTURE_BOOL(inplace);
  const Long y_size = CE_SIZE(dft_size);
  if (inplace && x_stride == 1 && y_stride == 1) {
    struct NS(RealTensor) *x = NS(Random_RealTensor_new2)(batch_size, dft_size);
    struct NS(ComplexTensor) *y0 = NS(ComplexTensor_new2)(batch_size, y_size);
    for (Long i = 0; i < batch_size; ++i) {
      ARRAY_COPY(Real, dft_size, NS(RealTensor_at2)(x, i, 0),
                 (Real *)NS(ComplexTensor_at2)(y0, i, 0));
    }
    NS(RealToComplexFunction_evaluate)(func, (Real *)y0->data, y0->data);
    struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(y0);
    NS(RDftNaive_BatchDft_F)(dft_size, batch_size, x->data, 1, dft_size,
                             y1->data, 1, y_size);
    ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
    NS(RealTensor_delete)(x);
    NS(ComplexTensor_delete)(y0);
    NS(ComplexTensor_delete)(y1);
  } else {
    struct NS(RealTensor) *x =
        NS(Random_RealTensor_new3)(batch_size, dft_size, x_stride);
    struct NS(RealTensor) *x0 = NS(RealTensor_copy)(x);
    struct NS(ComplexTensor) *y0 =
        NS(ComplexTensor_new3)(batch_size, y_size, y_stride);
    const Long x_stride_1 = dft_size * x_stride;
    const Long y_stride_1 = y_size * y_stride;
    NS(RealToComplexFunction_evaluate)(func, x0->data, y0->data);
    struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(y0);
    NS(RDftNaive_BatchDft_F)(dft_size, batch_size, x->data, x_stride,
                             x_stride_1, y1->data, y_stride, y_stride_1);
    ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
    NS(RealTensor_delete)(x);
    NS(RealTensor_delete)(x0);
    NS(ComplexTensor_delete)(y0);
    NS(ComplexTensor_delete)(y1);
  }
}

void NS_(OuterBatchDft_BTest)(struct NS(ComplexToRealFunction) *func,
                              Long dft_size, Long batch_size, Long x_stride,
                              Long y_stride, bool inplace) {
  assert(func);
  assert(dft_size >= 2);
  assert(batch_size >= 1);
  assert(x_stride >= 1);
  assert(y_stride >= 1);
  CAPTURE_LONG(dft_size);
  CAPTURE_LONG(batch_size);
  CAPTURE_LONG(x_stride);
  CAPTURE_LONG(y_stride);
  CAPTURE_BOOL(inplace);
  const Long x_size = CE_SIZE(dft_size);
  if (inplace && x_stride == 1 && y_stride == 1) {
    struct NS(ComplexTensor) *x =
        NS(Random_ConjugateEvenTensor_new2)(batch_size, dft_size);
    const Long y_size = 2 * x_size;
    struct NS(RealTensor) *y0 = NS(Random_RealTensor_new2)(batch_size, y_size);
    struct NS(RealTensor) *y1 = NS(RealTensor_zerosLike)(y0);
    for (Long i = 0; i < batch_size; ++i) {
      ARRAY_COPY(Complex, x_size, NS(ComplexTensor_at2)(x, i, 0),
                 (Complex *)NS(RealTensor_at2)(y0, i, 0));
      ARRAY_COPY(Complex, x_size, NS(ComplexTensor_at2)(x, i, 0),
                 (Complex *)NS(RealTensor_at2)(y1, i, 0));
    }
    NS(ComplexToRealFunction_evaluate)
    (func, (Complex *)y0->data, y0->data);
    NS(RDftNaive_BatchDft_B)(dft_size, batch_size, x->data, 1, x_size, y1->data,
                             1, y_size);
    ASSERT_REALTENSOR_AEQ(y0, y1);
    NS(RealTensor_delete)(x);
    NS(ComplexTensor_delete)(y0);
    NS(ComplexTensor_delete)(y1);
  } else {
    Long sizes[] = {batch_size, dft_size, x_stride};
    struct NS(ComplexTensor) *x =
        NS(Random_ConjugateEvenTensor_new_)(3, sizes, 1);
    struct NS(ComplexTensor) *x0 = NS(ComplexTensor_copy)(x);
    struct NS(RealTensor) *y0 =
        NS(RealTensor_new3)(batch_size, dft_size, y_stride);
    struct NS(RealTensor) *y1 = NS(RealTensor_zerosLike)(y0);
    const Long x_stride_1 = x_size * x_stride;
    const Long y_stride_1 = dft_size * y_stride;
    NS(ComplexToRealFunction_evaluate)(func, x0->data, y0->data);
    NS(RDftNaive_BatchDft_B)(dft_size, batch_size, x->data, x_stride,
                             x_stride_1, y1->data, y_stride, y_stride_1);
    ASSERT_REALTENSOR_AEQ(y0, y1);
    NS(RealTensor_delete)(x);
    NS(RealTensor_delete)(x0);
    NS(ComplexTensor_delete)(y0);
    NS(ComplexTensor_delete)(y1);
  }
}
