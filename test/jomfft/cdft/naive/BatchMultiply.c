#include <jomfft/math/Math.h>

#include "Dft.h"

void NS(CDftNaive_BatchMultiply)(bool backward, Long batch_size,
                                 const Complex *w, Long w_stride,
                                 const Complex *x, Long x_stride, Complex *y,
                                 Long y_stride) {
  if (backward) {
    for (Long i = 0; i < batch_size;
         ++i, w += w_stride, x += x_stride, y += y_stride) {
      *y = C_MUL(*x, C_CONJ(*w));
    }
  } else {
    for (Long i = 0; i < batch_size;
         ++i, w += w_stride, x += x_stride, y += y_stride) {
      *y = C_MUL(*x, *w);
    }
  }
}
