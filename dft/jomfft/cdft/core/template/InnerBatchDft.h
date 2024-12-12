#include "Common.h"

#define CDFT_LOAD(i, z) *(z) = CV_LOAD(x + x_stride * (i))
#define CDFT_STORE(i, z) CV_STORE(y + y_stride * (i), (z))

void FUNCTION(Long batch_size, Complex *x, Long x_stride, Complex *y,
              Long y_stride) {
  assert(batch_size % CV_SIZE == 0);
  Complex *xend = x + batch_size;
  for (; x != xend; x += CV_SIZE, y += CV_SIZE) {
#include TEMPLATE_FILE
  }
}
