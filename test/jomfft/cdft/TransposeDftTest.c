#include "TransposeDftTest.h"

#include <jomfft/util/random/ComplexTensor.h>

#include "naive/BatchDft.h"

#define NS_(n) NS(CDft_##n)

void NS_(TransposeDft_0Test)(struct NS(ComplexFunction) *func, bool backward,
                             Long dft_size, Long batch_size) {
  CAPTURE_BOOL(backward);
  CAPTURE_LONG(dft_size);
  CAPTURE_LONG(batch_size);
  struct NS(ComplexTensor) *x =
      NS(Random_ComplexTensor_new2)(dft_size, batch_size);
  struct NS(ComplexTensor) *x0 = NS(ComplexTensor_copy)(x);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_new2)(batch_size, dft_size);
  NS(ComplexFunction_evaluate)(func, x0->data, y0->data);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(y0);
  NS(CDftNaive_BatchDft)(backward, dft_size, batch_size, x->data, batch_size, 1,
                         y1->data, 1, dft_size);
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(x0);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
}

void NS_(TransposeDft_1Test)(struct NS(ComplexFunction) *func, bool backward,
                             Long dft_size, Long batch_size) {
  CAPTURE_BOOL(backward);
  CAPTURE_LONG(dft_size);
  CAPTURE_LONG(batch_size);
  struct NS(ComplexTensor) *x =
      NS(Random_ComplexTensor_new2)(batch_size, dft_size);
  struct NS(ComplexTensor) *x0 = NS(ComplexTensor_copy)(x);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_new2)(dft_size, batch_size);
  NS(ComplexFunction_evaluate)(func, x0->data, y0->data);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(y0);
  NS(CDftNaive_BatchDft)(backward, dft_size, batch_size, x->data, 1, dft_size,
                         y1->data, batch_size, 1);
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(x0);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
}
