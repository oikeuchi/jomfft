#include "SplitComplexCopy.h"

#include "../math/Math.h"

void NS(SplitComplex_Copy_F)(Long size, const Real *x_r, const Real *x_i,
                             Long x_stride, Complex *y, Long y_stride) {
  for (Long i = 0; i < size; ++i) {
    y[y_stride * i] = Complex_(x_r[x_stride * i], x_i[x_stride * i]);
  }
}

void NS(SplitComplex_Copy_B)(Long size, const Complex *x, Long x_stride,
                             Real *y_r, Real *y_i, Long y_stride) {
  for (Long i = 0; i < size; ++i) {
    y_r[y_stride * i] = C_REAL(x[x_stride * i]);
    y_i[y_stride * i] = C_IMAG(x[x_stride * i]);
  }
}

void NS(SplitComplex_CopyReal_F)(Long size, const Real *x_r, Long x_stride,
                                 Complex *y, Long y_stride) {
  for (Long i = 0; i < size; ++i) {
    y[y_stride * i] = Complex_R(x_r[x_stride * i]);
  }
}

void NS(SplitComplex_CopyReal_B)(Long size, const Complex *x, Long x_stride,
                                 Real *y_r, Long y_stride) {
  for (Long i = 0; i < size; ++i) {
    y_r[y_stride * i] = C_REAL(x[x_stride * i]);
  }
}
