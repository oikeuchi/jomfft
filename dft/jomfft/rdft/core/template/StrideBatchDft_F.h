#define USE_COMPLEX_CONJ 0
#include "Common.h"

#define RDFT_LOAD(i) RV_LOAD_S(x + x_stride_0 * (i), x_stride_1)
#define RDFT_STORE_RI(i, z_r, z_i)                                             \
  RV_STORE_RI_S(y + y_stride_0 * (i), y_stride_1, (z_r), (z_i))
#define RDFT_STORE_RI_0_(n, z0) RDFT_STORE_RI(0, z0, RV_ZERO())
#define RDFT_STORE_RI_h_(n, zh) RDFT_STORE_RI((n) / 2, zh, RV_ZERO())
#define RDFT_STORE_RI_0(n, z0, zh)                                             \
  do {                                                                         \
    RDFT_STORE_RI_0_((n), (z0));                                               \
    RDFT_STORE_RI_h_((n), (zh));                                               \
  } while (0)

void FUNCTION(Long batch_size, Real *x, Long x_stride_0, Long x_stride_1,
              Complex *y, Long y_stride_0, Long y_stride_1) {
  assert(batch_size % RV_SIZE == 0);
  Real *xend = x + x_stride_1 * batch_size;
  const Long x_stride_n = x_stride_1 * RV_SIZE;
  const Long y_stride_n = y_stride_1 * RV_SIZE;
  for (; x != xend; x += x_stride_n, y += y_stride_n) {
#include TEMPLATE_FILE
  }
}
