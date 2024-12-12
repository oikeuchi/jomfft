#include "BatchDft.h"

#include "Dft.h"

void NS(RDftNaive_BatchDft_F)(Long dft_size, Long batch_size, const Real *x,
                              Long x_stride_0, Long x_stride_1, Complex *y,
                              Long y_stride_0, Long y_stride_1) {
  for (Long i = 0; i < batch_size; ++i) {
    NS(RDftNaive_Dft_F)(dft_size, x + x_stride_1 * i, x_stride_0,
                        y + y_stride_1 * i, y_stride_0);
  }
}

void NS(RDftNaive_BatchDft_B)(Long dft_size, Long batch_size, const Complex *x,
                              Long x_stride_0, Long x_stride_1, Real *y,
                              Long y_stride_0, Long y_stride_1) {
  for (Long i = 0; i < batch_size; ++i) {
    NS(RDftNaive_Dft_B)(dft_size, x + x_stride_1 * i, x_stride_0,
                        y + y_stride_1 * i, y_stride_0);
  }
}
