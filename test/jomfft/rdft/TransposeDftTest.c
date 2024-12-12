#include "TransposeDftTest.h"

#include <jomfft/util/random/ConjugateEvenTensor.h>
#include <jomfft/util/random/RealTensor.h>

#include "naive/BatchDft.h"

#define NS_(n) NS(RDft_##n)

void NS_(TransposeDft_0_FTest)(struct NS(RealToComplexFunction) *func,
                               Long dft_size, Long batch_size) {
  assert(func);
  assert(dft_size >= 2);
  assert(batch_size >= 1);
  CAPTURE_LONG(dft_size);
  CAPTURE_LONG(batch_size);
  const Long y_size = CE_SIZE(dft_size);
  struct NS(RealTensor) *x = NS(Random_RealTensor_new2)(dft_size, batch_size);
  struct NS(RealTensor) *x0 = NS(RealTensor_copy)(x);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_new2)(batch_size, y_size);
  NS(RealToComplexFunction_evaluate)(func, x0->data, y0->data);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(y0);
  NS(RDftNaive_BatchDft_F)(dft_size, batch_size, x->data, batch_size, 1,
                           y1->data, 1, y_size);
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
  NS(RealTensor_delete)(x);
  NS(RealTensor_delete)(x0);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
}

void NS_(TransposeDft_1_FTest)(struct NS(RealToComplexFunction) *func,
                               Long dft_size, Long batch_size) {
  assert(func);
  assert(dft_size >= 2);
  assert(batch_size >= 1);
  CAPTURE_LONG(dft_size);
  CAPTURE_LONG(batch_size);
  const Long y_size = CE_SIZE(dft_size);
  struct NS(RealTensor) *x = NS(Random_RealTensor_new2)(batch_size, dft_size);
  struct NS(RealTensor) *x0 = NS(RealTensor_copy)(x);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_new2)(y_size, batch_size);
  NS(RealToComplexFunction_evaluate)(func, x0->data, y0->data);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(y0);
  NS(RDftNaive_BatchDft_F)(dft_size, batch_size, x->data, 1, dft_size, y1->data,
                           batch_size, 1);
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
  NS(RealTensor_delete)(x);
  NS(RealTensor_delete)(x0);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
}

void NS_(TransposeDft_0_BTest)(struct NS(ComplexToRealFunction) *func,
                               Long dft_size, Long batch_size) {
  assert(func);
  assert(dft_size >= 2);
  assert(batch_size >= 1);
  CAPTURE_LONG(dft_size);
  CAPTURE_LONG(batch_size);
  Long x_sizes[] = {dft_size, batch_size};
  struct NS(ComplexTensor) *x =
      NS(Random_ConjugateEvenTensor_new_)(2, x_sizes, 0);
  struct NS(ComplexTensor) *x0 = NS(ComplexTensor_copy)(x);
  struct NS(RealTensor) *y0 = NS(RealTensor_new2)(batch_size, dft_size);
  struct NS(RealTensor) *y1 = NS(RealTensor_zerosLike)(y0);
  NS(ComplexToRealFunction_evaluate)(func, x0->data, y0->data);
  NS(RDftNaive_BatchDft_B)(dft_size, batch_size, x->data, batch_size, 1,
                           y1->data, 1, dft_size);
  ASSERT_REALTENSOR_AEQ(y0, y1);
  NS(RealTensor_delete)(x);
  NS(RealTensor_delete)(x0);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
}

void NS_(TransposeDft_1_BTest)(struct NS(ComplexToRealFunction) *func,
                               Long dft_size, Long batch_size) {
  assert(func);
  assert(dft_size >= 2);
  assert(batch_size >= 1);
  CAPTURE_LONG(dft_size);
  CAPTURE_LONG(batch_size);
  const Long x_size = CE_SIZE(dft_size);
  Long x_sizes[] = {batch_size, dft_size};
  struct NS(ComplexTensor) *x =
      NS(Random_ConjugateEvenTensor_new_)(2, x_sizes, 1);
  struct NS(ComplexTensor) *x0 = NS(ComplexTensor_copy)(x);
  struct NS(RealTensor) *y0 = NS(RealTensor_new2)(dft_size, batch_size);
  struct NS(RealTensor) *y1 = NS(RealTensor_zerosLike)(y0);
  NS(ComplexToRealFunction_evaluate)(func, x0->data, y0->data);
  NS(RDftNaive_BatchDft_B)(dft_size, batch_size, x->data, 1, x_size, y1->data,
                           batch_size, 1);
  ASSERT_REALTENSOR_AEQ(y0, y1);
  NS(RealTensor_delete)(x);
  NS(RealTensor_delete)(x0);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
}
