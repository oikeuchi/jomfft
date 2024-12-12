#include "Common.h"

#define CDFT_LOAD(i, z) *(z) = CV_LOAD_S(x + x_stride_0 * (i), x_stride_1)
#define CDFT_STORE(i, z) z__[(i)] = (z)

void FUNCTION(Long batch_size, const Complex *w, Complex *x, Long x_stride_0,
              Long x_stride_1, Complex *y, Long y_stride) {
  assert(batch_size % CV_SIZE == 0);
  const Complex *wend = w + batch_size;
  for (; w != wend; w += CV_SIZE, x += x_stride_1 * CV_SIZE, y += CV_SIZE) {
    CV_VECTOR z__[2];
#include "Dft_2.h"
    CV_TRANSPOSE_2(&z__[0], &z__[1]);
    CV_VECTOR
    w__ = CV_LOAD(w);
    CV_STORE(y, CV_N_MUL(z__[0], w__));
    CV_STORE(y + y_stride, CV_N_MUL(z__[1], w__));
  }
}
