#include "ConjugateEvenCopy.h"

#include "../util/ConjugateEvenCopy.h"

struct NS(Complex_CopyCEData) {
  Long size;
  Long x_stride;
  Long y_stride;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct NS(Complex_CopyCEData) *Complex_CopyCEData_new(Long size,
                                                             Long x_stride,
                                                             Long y_stride,
                                                             int num_threads) {
  struct NS(Complex_CopyCEData) *out = NEW(struct NS(Complex_CopyCEData));
  *out = (struct NS(Complex_CopyCEData)){
      .size = size, .x_stride = x_stride, .y_stride = y_stride};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static void Complex_CopyCEData_evaluate(struct NS(ComplexFunction) *p,
                                        Complex *x, Complex *y) {
  struct NS(Complex_CopyCEData) *data =
      (struct NS(Complex_CopyCEData) *)NS(ComplexFunction_data)(p);
#if USE_OPENMP
  NS(Complex_CopyCE_)(data->size, x, data->x_stride, y, data->y_stride,
                      data->num_threads);
#else
  NS(Complex_CopyCE)(data->size, x, data->x_stride, y, data->y_stride);
#endif
}

struct NS(ComplexFunction) *NS(Complex_CopyCE_new)(Long size, Long x_stride,
                                                   Long y_stride,
                                                   int num_threads) {
  return NS(ComplexFunction_new)(
      Complex_CopyCEData_evaluate, jomfftDestructPointer,
      Complex_CopyCEData_new(size, x_stride, y_stride, num_threads));
}
