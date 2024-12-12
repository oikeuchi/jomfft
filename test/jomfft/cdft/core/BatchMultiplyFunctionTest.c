#include "BatchMultiplyFunctionTest.h"

#include <jomfft/util/random/ComplexTensor.h>

#include "../naive/BatchMultiply.h"

void NS(CDftCore_BatchMultiplyFunctionTest)(NS(CDftCore_BatchMultiply_fn) func,
                                            bool backward, Long batch_size) {
  CAPTURE_BOOL(backward);
  CAPTURE_LONG(batch_size);
  const Long stride = 2;
  struct NS(ComplexTensor) *w = NS(Random_ComplexTensor_new1)(batch_size);
  struct NS(ComplexTensor) *x =
      NS(Random_ComplexTensor_new2)(batch_size, stride);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_zerosLike)(x);
  func(batch_size, w->data, x->data, stride, y0->data, stride);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(x);
  NS(CDftNaive_BatchMultiply)(backward, batch_size, w->data, 1, x->data, stride,
                              y1->data, stride);
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
  NS(ComplexTensor_delete)(w);
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
}
