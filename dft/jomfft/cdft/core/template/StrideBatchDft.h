#include "Common.h"

#define CDFT_LOAD(i, z) *(z) = CV_LOAD_S(x + x_stride_0 * (i), x_stride_1)
#define CDFT_STORE(i, z) CV_STORE_S(y + y_stride_0 * (i), y_stride_1, (z))

void FUNCTION(Long batch_size, Complex *x, Long x_stride_0, Long x_stride_1,
              Complex *y, Long y_stride_0, Long y_stride_1) {
  assert(batch_size % CV_SIZE == 0);
  Complex *xend = x + x_stride_1 * batch_size;
  const Long x_stride_n = x_stride_1 * CV_SIZE;
  const Long y_stride_n = y_stride_1 * CV_SIZE;
  for (; x != xend; x += x_stride_n, y += y_stride_n) {
#include TEMPLATE_FILE
  }
}
