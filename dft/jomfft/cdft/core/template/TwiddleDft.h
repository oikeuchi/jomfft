#include "Common.h"

#define CDFT_LOAD(i, z)                                                        \
  do {                                                                         \
    *(z) = CV_LOAD(x + stride * (i));                                          \
    if (i != 0) {                                                              \
      *(z) = CV_N_MUL(*(z), CV_LOAD(w + stride * (i - 1)));                    \
    }                                                                          \
  } while (0)

#define CDFT_STORE(i, z) CV_STORE(y + stride * (i), (z))

void FUNCTION(Long batch_size, const Complex *w, Complex *x, Complex *y,
              Long stride) {
  assert(batch_size % CV_SIZE == 0);
  const Complex *wend = w + batch_size;
  for (; w != wend; w += CV_SIZE, x += CV_SIZE, y += CV_SIZE) {
#include TEMPLATE_FILE
  }
}
