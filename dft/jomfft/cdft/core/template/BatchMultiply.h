#include "Common.h"

void FUNCTION(Long batch_size, const Complex *w, Complex *x, Long x_stride,
              Complex *y, Long y_stride) {
  assert(batch_size % CV_SIZE == 0);
  const Complex *wend = w + batch_size;
  const Long x_stride_n = x_stride * CV_SIZE;
  const Long y_stride_n = y_stride * CV_SIZE;
  for (; w != wend; w += CV_SIZE, x += x_stride_n, y += y_stride_n) {
    CV_STORE_S(y, y_stride, CV_N_MUL(CV_LOAD_S(x, x_stride), CV_LOAD(w)));
  }
}
