#include "ComplexTranspose.h"

#include "../util/ComplexTranspose.h"

struct NS(Complex_TransposeData) {
  Long size;
  Long stride;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct NS(Complex_TransposeData) *
Complex_TransposeData_new(Long size, Long stride, int num_threads) {
  assert(size > 0);
  assert(stride > 0);
  struct NS(Complex_TransposeData) *out = NEW(struct NS(Complex_TransposeData));
  *out = (struct NS(Complex_TransposeData)){.size = size, .stride = stride};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static void Complex_Transpose_evaluate(struct NS(ComplexFunction) *p,
                                       Complex *x, Complex *y) {
  struct NS(Complex_TransposeData) *data =
      (struct NS(Complex_TransposeData) *)NS(ComplexFunction_data)(p);
  assert(data);
  assert(x == y);
  (void)y;
#if USE_OPENMP
  NS(Complex_Transpose_)(data->size, x, data->stride, data->num_threads);
#else
  NS(Complex_Transpose)(data->size, x, data->stride);
#endif
}

struct NS(ComplexFunction) *NS(Complex_Transpose_new)(Long size, Long stride) {
  return NS(Complex_Transpose_new_)(size, stride, 1);
}

struct NS(ComplexFunction) *NS(Complex_Transpose_new_)(Long size, Long stride,
                                                       int num_threads) {
  return NS(ComplexFunction_new)(
      Complex_Transpose_evaluate, jomfftDestructPointer,
      Complex_TransposeData_new(size, stride, num_threads));
}
