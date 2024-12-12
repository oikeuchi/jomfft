#include "Common.h"

void FUNCTION(Long batch_size, Complex *x0, Complex *x1, Complex *y0,
              Complex *y1, Long stride) {
  assert(batch_size % CV_SIZE == 0);
  const Long stride_n = stride * CV_SIZE;
  Complex *y0_end = y0 + batch_size;
  for (; y0 != y0_end; x0 += stride_n, x1 += stride_n, y0 += CV_SIZE) {
    y1 -= CV_SIZE;
    CV_VECTOR z0 = CV_LOAD_S(x0, stride);
    CV_VECTOR z1 = CV_LOAD_S(x1, stride);
    CV_MULI_DFT2(&z0, &z1);
    z1 = CV_REVERSE(CV_CONJ(z1));
    CV_STORE(y0, z0);
    CV_STORE(y1, z1);
  }
}
