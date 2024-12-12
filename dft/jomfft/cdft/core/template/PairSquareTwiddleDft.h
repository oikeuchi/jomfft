#include "Common.h"

#define NUM_OF_COLUMNS (DFT_SIZE / CV_SIZE)

void FUNCTION(const Complex *w0, const Complex *w1, Complex *x, Complex *y) {
  assert(DFT_SIZE % CV_SIZE == 0);
  w0 += DFT_SIZE;
  const Complex *w1_0 = w1;
  const Complex *w1_1 = w1 + DFT_SIZE;
  Complex *x0 = x;
  Complex *x1 = x + DFT_SIZE;
  Complex *y0 = y;
  Complex *y1 = y + DFT_SIZE;
  const Long stride = 2 * DFT_SIZE;
  for (int i = 0; i < NUM_OF_COLUMNS; ++i, w0 += CV_SIZE, w1_0 += CV_SIZE,
           w1_1 += CV_SIZE, x0 += CV_SIZE, x1 += CV_SIZE, y0 += CV_SIZE,
           y1 += CV_SIZE) {
    CV_VECTOR w0__ = CV_LOAD(w0);
    CV_VECTOR z0__[DFT_SIZE];
    CV_VECTOR z1__[DFT_SIZE];
    for (int j = 0; j < DFT_SIZE; ++j) {
      CV_VECTOR u0 = CV_LOAD(x0 + stride * j);
      CV_VECTOR u1 = CV_LOAD(x1 + stride * j);
      u1 = CV_N_MUL(u1, w0__);
      CV_DFT2(&u0, &u1);
      z0__[j] = u0;
      z1__[j] = u1;
    }
#define CDFT_LOAD(i, z) *(z) = CV_N_MUL(z0__[i], CV_LOAD(w1_0 + stride * (i)))
#define CDFT_STORE(i, z) CV_STORE(y0 + stride * (i), (z))

#include TEMPLATE_FILE

#undef CDFT_LOAD
#undef CDFT_STORE

#define CDFT_LOAD(i, z) *(z) = CV_N_MUL(z1__[i], CV_LOAD(w1_1 + stride * (i)))
#define CDFT_STORE(i, z) CV_STORE(y1 + stride * (i), (z))

#include TEMPLATE_FILE

#undef CDFT_LOAD
#undef CDFT_STORE
  }
}
