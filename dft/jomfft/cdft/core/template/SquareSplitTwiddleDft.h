#include "Common.h"

#define CDFT_LOAD(i, z)                                                        \
  do {                                                                         \
    *(z) = CV_LOAD(x + stride * (i));                                          \
    if (i != 0) {                                                              \
      *(z) = CV_N_MUL(                                                         \
          *(z), CV_MUL_SCALAR(CV_LOAD(w0 + w_stride * (i - 1)), v[(i)]));      \
    }                                                                          \
  } while (0)

#define CDFT_STORE(i, z) CV_STORE(y + stride * (i), (z))

void FUNCTION(Long batch_size, const Complex *v, const Complex *w, int w_stride,
              Complex *x, Complex *y, Long stride) {
  assert(batch_size % DFT_SIZE == 0);
  assert(DFT_SIZE % CV_SIZE == 0);
  Complex *xend = x + batch_size;
  for (; x != xend; v += DFT_SIZE) {
    const Complex *w0 = w;
    for (int i = 0; i < DFT_SIZE;
         i += CV_SIZE, w0 += CV_SIZE, x += CV_SIZE, y += CV_SIZE) {
#include TEMPLATE_FILE
    }
  }
}
