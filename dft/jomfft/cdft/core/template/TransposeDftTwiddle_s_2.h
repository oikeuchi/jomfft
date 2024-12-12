#include "Common.h"

#define CDFT_LOAD(i, z) *(z) = CV_LOAD_S(x + x_stride_0 * (i), x_stride_1)
#define CDFT_STORE(i, z) z__[(i)] = (z)
#define NUM_OF_COLUMNS (DFT_SIZE / 2)

void FUNCTION(Long batch_size, const Complex *w, Complex *x, Long x_stride_0,
              Long x_stride_1, Complex *y, Long y_stride) {
  assert(DFT_SIZE % 2 == 0);
  assert(batch_size % DFT_SIZE == 0);
  const Complex *wend = w + batch_size;
  for (; w != wend; w += DFT_SIZE, y += DFT_SIZE) {
    CV_VECTOR zz__[DFT_SIZE * NUM_OF_COLUMNS];
    CV_VECTOR *z__ = zz__;
    for (int i = 0; i < NUM_OF_COLUMNS;
         ++i, x += x_stride_1 * 2, z__ += DFT_SIZE) {
#include TEMPLATE_FILE
    }
    z__ = zz__;
    for (int i = 0; i < NUM_OF_COLUMNS; ++i) {
      const Complex *w0 = w;
      Complex *y0 = y + y_stride * (2 * i);
      Complex *y1 = y + y_stride * (2 * i + 1);
      for (int j = 0; j < NUM_OF_COLUMNS;
           ++j, w0 += 2, y0 += 2, y1 += 2, z__ += 2) {
        CV_TRANSPOSE_2(&z__[0], &z__[1]);
        CV_VECTOR w0__ = CV_LOAD(w0);
        CV_STORE(y0, CV_N_MUL(z__[0], w0__));
        CV_STORE(y1, CV_N_MUL(z__[1], w0__));
      }
    }
  }
}
