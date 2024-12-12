#include "Common.h"

void FUNCTION(Long batch_size, const Complex *w, Complex *x0, Complex *x1,
              Complex *y0, Complex *y1) {
  assert(batch_size % CV_SIZE == 0);
  const Complex *wend = w + batch_size;
  for (; w != wend; w += CV_SIZE, x0 += CV_SIZE, y0 += CV_SIZE) {
    x1 -= CV_SIZE;
    y1 -= CV_SIZE;
    CV_VECTOR z0 = CV_LOAD(x0);
    CV_VECTOR z1 = CV_LOAD(x1);
    z1 = CV_REVERSE(z1);
    CV_CONJ_DFT2(&z0, &z1);
    z1 = CV_MUL_CONJ(z1, CV_LOAD(w));
    CV_MULI_DFT2(&z0, &z1);
    z1 = CV_REVERSE(CV_CONJ(z1));
    CV_STORE(y0, z0);
    CV_STORE(y1, z1);
  }
}
