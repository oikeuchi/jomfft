#include "TwiddleDft.h"

#include <jomfft/math/Math.h>

#include "Dft.h"

void NS(CDftNaive_TwiddleDft)(bool backward, Long dft_size, Long twiddle_size,
                              const Complex *x, Complex *y) {
  Real sign = (Real)(backward ? 1 : -1);
  const Real theta = (Real)(sign * 2 * M_PI) / (dft_size * twiddle_size);
  const Long stride = twiddle_size;
  Complex *z = ARRAY_NEW(Complex, dft_size);
  for (Long i = 0; i < twiddle_size; ++i) {
    for (Long j = 0; j < dft_size; ++j) {
      z[j] =
          C_MUL(NS(Complex_fromArgument)(theta * (i * j)), x[i + stride * j]);
    }
    NS(CDftNaive_Dft)(backward, dft_size, z, 1, y + i, stride);
  }
  ARRAY_DELETE(z);
}
