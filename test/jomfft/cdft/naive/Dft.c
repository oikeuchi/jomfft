#include "Dft.h"

#include <jomfft/math/Math.h>
#include <jomfft/util/ComplexCopy.h>

void NS(CDftNaive_Dft)(bool backward, Long dft_size, const Complex *x,
                       Long x_stride, Complex *y, Long y_stride) {
  const Real theta = (Real)(C_SIGN(backward) * 2 * M_PI / dft_size);
  for (Long i = 0; i < dft_size; ++i) {
    double sum_r = 0;
    double sum_i = 0;
    for (Long j = 0; j < dft_size; ++j) {
      Complex z =
          C_MUL(NS(Complex_fromArgument)(theta * i * j), x[x_stride * j]);
      sum_r += C_REAL(z);
      sum_i += C_IMAG(z);
    }
    y[y_stride * i] = Complex_(sum_r, sum_i);
  }
}

void NS(CDftNaive_Dft_)(bool backward, Long dft_size, Complex *x,
                        Long x_stride) {
  Complex *temp = ARRAY_NEW(Complex, dft_size);
  NS(CDftNaive_Dft)(backward, dft_size, x, x_stride, temp, 1);
  NS(Complex_Copy)(dft_size, temp, 1, x, x_stride);
  ARRAY_DELETE(temp);
}
