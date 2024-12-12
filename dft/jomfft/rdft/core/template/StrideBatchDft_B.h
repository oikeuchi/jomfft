#define USE_COMPLEX_CONJ 1
#include "Common.h"

#define RDFT_LOAD_RI(i, z_r, z_i)                                              \
  RV_LOAD_RI_S(x + x_stride_0 * (i), x_stride_1, z_r, z_i)
#define RDFT_LOAD_RI_0_(n) RV_LOAD_S((Real *)x, 2 * x_stride_1)
#define RDFT_LOAD_RI_h_(n)                                                     \
  RV_LOAD_S((Real *)(x + x_stride_0 * (n) / 2), 2 * x_stride_1)
#define RDFT_LOAD_RI_0(n, z0, zh)                                              \
  do {                                                                         \
    *(z0) = RDFT_LOAD_RI_0_(n);                                                \
    *(zh) = RDFT_LOAD_RI_h_(n);                                                \
  } while (0)
#define RDFT_STORE(i, z) RV_STORE_S(y + y_stride_0 * (i), y_stride_1, (z))

void FUNCTION(Long batch_size, Complex *x, Long x_stride_0, Long x_stride_1,
              Real *y, Long y_stride_0, Long y_stride_1) {
  assert(batch_size % RV_SIZE == 0);
  Complex *xend = x + x_stride_1 * batch_size;
  const Long x_stride_n = x_stride_1 * RV_SIZE;
  const Long y_stride_n = y_stride_1 * RV_SIZE;
  for (; x != xend; x += x_stride_n, y += y_stride_n) {
#include TEMPLATE_FILE
  }
}
