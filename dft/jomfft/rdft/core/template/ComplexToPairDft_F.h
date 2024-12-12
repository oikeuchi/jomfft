#include "Common.h"

void FUNCTION(Long batch_size, Complex *x0, Complex *x1, Complex *y0,
              Complex *y1, Long stride) {
  assert(batch_size % CV_SIZE == 0);
  const Long stride_n = stride * CV_SIZE;
  Complex *x0_end = x0 + batch_size;
  for (; x0 != x0_end; x0 += CV_SIZE, y0 += stride_n, y1 += stride_n) {
    x1 -= CV_SIZE;
    CV_VECTOR z0 = CV_LOAD(x0);
    CV_VECTOR z1 = CV_LOAD(x1);
    z1 = CV_REVERSE(z1);
    CV_CONJ_DFT2(&z0, &z1);
    z0 = RV_MUL(RV_SCALAR(0.5), z0);
    z1 = RV_MUL(RV_SCALAR(0.5), z1);
    z1 = CV_MULI_CONJ(z1);
    CV_STORE_S(y0, stride, z0);
    CV_STORE_S(y1, stride, z1);
  }
}
