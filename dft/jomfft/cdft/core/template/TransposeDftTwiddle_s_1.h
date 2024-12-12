#include "Common.h"

#define CDFT_LOAD(i, z) *(z) = CV_LOAD_S(x + x_stride_0 * (i), x_stride_1)
#define CDFT_STORE(i, z) z__[(i)] = (z)

void FUNCTION(Long batch_size, const Complex *w, Complex *x, Long x_stride_0,
              Long x_stride_1, Complex *y, Long y_stride) {
  assert(batch_size % DFT_SIZE == 0);
  const Complex *wend = w + batch_size;
  for (; w != wend; w += DFT_SIZE, y += DFT_SIZE) {
    CV_VECTOR zz__[DFT_SIZE * DFT_SIZE];
    CV_VECTOR *z__ = zz__;
    for (int i = 0; i < DFT_SIZE; ++i, x += x_stride_1, z__ += DFT_SIZE) {
#include TEMPLATE_FILE
    }
    z__ = zz__;
    for (int i = 0; i < DFT_SIZE; ++i) {
      const Complex *w0 = w;
      Complex *y0 = y + y_stride * i;
      for (int j = 0; j < DFT_SIZE; ++j, ++w0, ++y0, ++z__) {
        CV_STORE(y0, CV_N_MUL(*z__, CV_LOAD(w0)));
      }
    }
  }
}
