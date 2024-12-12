#include "Dft.h"

#include <jomfft/math/Math.h>
#include <jomfft/util/RealCopy.h>

void NS(RDftNaive_Dft_F)(Long dft_size, const Real *x, Long x_stride,
                         Complex *y, Long y_stride) {
  const Long n_2 = dft_size / 2 + 1;
  const Real theta = (Real)(-2 * M_PI / dft_size);
  for (Long i = 0; i < n_2; ++i) {
    double sum_r = 0;
    double sum_i = 0;
    for (Long j = 0; j < dft_size; ++j) {
      Complex z =
          C_MUL_R(NS(Complex_fromArgument)(theta * i * j), x[x_stride * j]);
      sum_r += C_REAL(z);
      sum_i += C_IMAG(z);
    }
    y[y_stride * i] = Complex_(sum_r, sum_i);
  }
}

void NS(RDftNaive_Dft_B)(Long dft_size, const Complex *x, Long x_stride,
                         Real *y, Long y_stride) {
  const Long n_2_ = (dft_size + 1) / 2;
  const Real theta = (Real)(2 * M_PI / dft_size);
  for (Long i = 0; i < dft_size; ++i) {
    double sum = 0;
    sum += (double)C_REAL(x[0]);
    for (Long j = 1; j < n_2_; ++j) {
      Complex z =
          C_MUL(NS(Complex_fromArgument)(theta * i * j), x[x_stride * j]);
      sum += 2 * C_REAL(z);
    }
    if (dft_size % 2 == 0) {
      if (i % 2) {
        sum -= C_REAL(x[x_stride * n_2_]);
      } else {
        sum += C_REAL(x[x_stride * n_2_]);
      }
    }
    y[y_stride * i] = (Real)sum;
  }
}
