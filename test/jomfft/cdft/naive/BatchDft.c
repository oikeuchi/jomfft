#include "BatchDft.h"

#include "Dft.h"

void NS(CDftNaive_BatchDft)(bool backward, Long dft_size, Long batch_size,
                            const Complex *x, Long x_stride_0, Long x_stride_1,
                            Complex *y, Long y_stride_0, Long y_stride_1) {
  for (Long i = 0; i < batch_size; ++i) {
    NS(CDftNaive_Dft)(backward, dft_size, x + x_stride_1 * i, x_stride_0,
                      y + y_stride_1 * i, y_stride_0);
  }
}
