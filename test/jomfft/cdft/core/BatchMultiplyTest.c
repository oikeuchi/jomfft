#include "BatchMultiplyTest.h"

#include <jomfft/cdft/core/BatchMultiply.h>
#include <jomfft/util/random/ComplexTensor.h>

#include "../naive/BatchMultiply.h"

static void CDftCore_BatchMultiplyTest__(bool backward, Long batch_size,
                                         Long x_stride, Long y_stride,
                                         int num_threads) {
  CAPTURE_BOOL(backward);
  CAPTURE_LONG(batch_size);
  CAPTURE_LONG(x_stride);
  CAPTURE_LONG(y_stride);
  CAPTURE_INT(num_threads);
  struct NS(ComplexTensor) *w = NS(Random_ComplexTensor_new1)(batch_size);
  struct NS(ComplexTensor) *x =
      NS(Random_ComplexTensor_new2)(batch_size, x_stride);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_new2)(batch_size, y_stride);
  struct NS(ComplexFunction) *p = NS(CDftCore_BatchMultiply_new)(
      backward, batch_size, w->data, x_stride, y_stride, num_threads);
  NS(ComplexFunction_evaluate)(p, x->data, y0->data);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(y0);
  NS(CDftNaive_BatchMultiply)(backward, batch_size, w->data, 1, x->data,
                              x_stride, y1->data, y_stride);
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
  NS(ComplexFunction_delete)(p);
  NS(ComplexTensor_delete)(w);
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
}

static void CDftCore_BatchMultiplyTest_(bool backward, Long batch_size,
                                        Long x_stride, Long y_stride) {
  CDftCore_BatchMultiplyTest__(backward, batch_size, x_stride, y_stride, 1);
}

void NS(CDftCore_BatchMultiplyTest)() {
  CDftCore_BatchMultiplyTest_(false, 1, 1, 1);
  CDftCore_BatchMultiplyTest_(true, 3, 1, 2);
  CDftCore_BatchMultiplyTest_(true, 4, 2, 1);
  CDftCore_BatchMultiplyTest_(false, 5, 2, 2);
  CDftCore_BatchMultiplyTest_(false, 6, 1, 1);
  CDftCore_BatchMultiplyTest_(true, 7, 1, 1);
  CDftCore_BatchMultiplyTest_(false, 8, 1, 1);
  CDftCore_BatchMultiplyTest_(false, 16, 1, 1);
#if USE_OPENMP
  CDftCore_BatchMultiplyTest__(false, 16, 1, 1, 2);
  CDftCore_BatchMultiplyTest__(false, 17, 2, 2, 2);
  CDftCore_BatchMultiplyTest__(true, 16, 1, 1, 2);
  CDftCore_BatchMultiplyTest__(true, 17, 2, 2, 2);
#endif
}
