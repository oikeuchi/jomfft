#include "Common.h"

#define CDFT_LOAD(i, z) *(z) = CV_LOAD_S(x + x_stride_0 * (i), x_stride_1)
#define CDFT_STORE(i, z) z__[(i)] = (z)
#define NUM_OF_COLUMNS (DFT_SIZE / 8)

void FUNCTION(Long batch_size, const Complex *w, Complex *x, Long x_stride_0,
              Long x_stride_1, Complex *y, Long y_stride) {
  assert(DFT_SIZE % 8 == 0);
  assert(batch_size % DFT_SIZE == 0);
  const Complex *wend = w + batch_size;
  for (; w != wend; w += DFT_SIZE, y += DFT_SIZE) {
    CV_VECTOR
    zz__[DFT_SIZE * NUM_OF_COLUMNS];
    CV_VECTOR *z__ = zz__;
    for (int i = 0; i < NUM_OF_COLUMNS;
         ++i, x += x_stride_1 * 8, z__ += DFT_SIZE) {
#include TEMPLATE_FILE
    }
    z__ = zz__;
    for (int i = 0; i < NUM_OF_COLUMNS; ++i) {
      const Complex *w0 = w;
      Complex *y0 = y + y_stride * (4 * i);
      Complex *y1 = y + y_stride * (4 * i + 1);
      Complex *y2 = y + y_stride * (4 * i + 2);
      Complex *y3 = y + y_stride * (4 * i + 3);
      Complex *y4 = y + y_stride * (4 * i + 4);
      Complex *y5 = y + y_stride * (4 * i + 5);
      Complex *y6 = y + y_stride * (4 * i + 6);
      Complex *y7 = y + y_stride * (4 * i + 7);
      for (int j = 0; j < NUM_OF_COLUMNS; ++j, w0 += 8, y0 += 8, y1 += 8,
               y2 += 8, y3 += 8, y4 += 8, y5 += 8, y6 += 8, y7 += 8, z__ += 8) {
        CV_TRANSPOSE_8(&z__[0], &z__[1], &z__[2], &z__[3], &z__[4], &z__[5],
                       &z__[6], &z__[7]);
        CV_VECTOR
        w0__ = CV_LOAD(w0);
        CV_STORE(y0, CV_N_MUL(z__[0], w0__));
        CV_STORE(y1, CV_N_MUL(z__[1], w0__));
        CV_STORE(y2, CV_N_MUL(z__[2], w0__));
        CV_STORE(y3, CV_N_MUL(z__[3], w0__));
        CV_STORE(y4, CV_N_MUL(z__[4], w0__));
        CV_STORE(y5, CV_N_MUL(z__[5], w0__));
        CV_STORE(y6, CV_N_MUL(z__[6], w0__));
        CV_STORE(y7, CV_N_MUL(z__[7], w0__));
      }
    }
  }
}
