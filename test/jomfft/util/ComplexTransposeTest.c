#include "ComplexTransposeTest.h"

#include <jomfft/util/ComplexTranspose.h>
#include <jomfft/util/random/ComplexTensor.h>

void ComplexTransposeTest__(Long size, Long stride, int num_threads) {
  CAPTURE_LONG(size);
  CAPTURE_LONG(stride);
  CAPTURE_INT(num_threads);
  struct NS(ComplexTensor) *x =
      NS(Random_ComplexTensor_new2)(size, size * stride);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_copy)(x);
  NS(Complex_Transpose_)(size, y0->data, y0->shape->sizes[1], num_threads);

  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_copy)(x);
  for (Long i = 1; i < size; ++i) {
    for (Long j = 0; j < i; ++j) {
      Complex t0 = NS(ComplexTensor_get2)(y1, i, j);
      Complex t1 = NS(ComplexTensor_get2)(y1, j, i);
      NS(ComplexTensor_set2)(y1, i, j, t1);
      NS(ComplexTensor_set2)(y1, j, i, t0);
    }
  }
  ASSERT_COMPLEXTENSOR_EQ(y0, y1);
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
}

void ComplexTransposeTest_(Long size, Long stride) {
  ComplexTransposeTest__(size, stride, 1);
}

void NS(ComplexTransposeTest)() {
  ComplexTransposeTest_(2, 1);
  ComplexTransposeTest_(5, 1);
  ComplexTransposeTest_(16, 1);
  ComplexTransposeTest_(16, 2);
  ComplexTransposeTest_(256, 1);
  ComplexTransposeTest_(151, 1);
  ComplexTransposeTest_(243, 1);
#if USE_OPENMP
  ComplexTransposeTest__(256, 1, 2);
  ComplexTransposeTest__(128, 2, 2);
  ComplexTransposeTest__(256, 1, omp_get_max_threads());
  ComplexTransposeTest__(128, 2, omp_get_max_threads());
  ComplexTransposeTest__(151, 1, 2);
  ComplexTransposeTest__(243, 2, 2);
  ComplexTransposeTest__(243, 1, omp_get_max_threads());
  ComplexTransposeTest__(151, 2, omp_get_max_threads());
#endif
}
