#include <jomfft/math/constants/SinCos_9.h>

#ifndef RDFT_12_LOAD_RI_0_
#define RDFT_12_LOAD_RI_0_ RDFT_LOAD_RI_0_
#endif

#ifndef RDFT_12_LOAD_RI_h_
#define RDFT_12_LOAD_RI_h_ RDFT_LOAD_RI_h_
#endif

#ifndef RDFT_12_LOAD_RI
#define RDFT_12_LOAD_RI RDFT_LOAD_RI
#endif

#ifndef RDFT_12_STORE
#define RDFT_12_STORE RDFT_STORE
#endif

#define RDFT_12_LOAD_RI_(i, z_r, z_i)                                          \
  do {                                                                         \
    if (i > (12 / 2)) {                                                        \
      RDFT_LOAD_RI(12 - i, z_r, z_i);                                          \
      *z_i = RV_NEGATE(*z_i);                                                  \
    } else {                                                                   \
      RDFT_LOAD_RI(i, z_r, z_i);                                               \
    }                                                                          \
  } while (0)

{
  const int i_indexes_12__[] = {0, 3, 6, 9, 4, 7, 10, 1, 8, 11, 2, 5};
  const int o_indexes_12__[] = {0, 9, 6, 3, 4, 1, 10, 7, 8, 5, 2, 11};
  RV_VECTOR z_12__[12];
#define RDFT_3_LOAD_RI_0_(n) RDFT_12_LOAD_RI_0_(12)
#define RDFT_3_LOAD_RI(i, x_r, x_i) RDFT_12_LOAD_RI_(i, x_r, x_i)
#define RDFT_3_INPUT_INDEX(i) i_indexes_12__[4 * (i)]
#define RDFT_3_STORE(i, x) z_12__[4 * (i)] = x
#include "Dft_B3.h"
#define RDFT_3_LOAD_RI_0_(n) RDFT_12_LOAD_RI_h_(12)
#define RDFT_3_LOAD_RI(i, x_r, x_i) RDFT_12_LOAD_RI_(i, x_r, x_i)
#define RDFT_3_INPUT_INDEX(i) i_indexes_12__[4 * (i) + 2]
#define RDFT_3_STORE(i, x) z_12__[4 * (i) + 1] = x
#include "Dft_B3.h"
#define SDFT_3_LOAD(i, x) RDFT_12_LOAD_RI_(i, &x[0], &x[1])
#define SDFT_3_INPUT_INDEX(i) i_indexes_12__[4 * (i) + 1]
#define SDFT_3_STORE(i, x)                                                     \
  do {                                                                         \
    z_12__[4 * (i) + 2] = x[0];                                                \
    z_12__[4 * (i) + 3] = x[1];                                                \
  } while (0)
#include "SplitDft_3.h"
#define RDFT_4_LOAD_RI_0(n, x_r, x_i) RDFT_LOAD_ARRAY_RI_0(&z_12__[0], x_r, x_i)
#define RDFT_4_LOAD_RI(i, x_r, x_i) RDFT_LOAD_ARRAY_RI(&z_12__[0], i, x_r, x_i)
#define RDFT_4_STORE RDFT_12_STORE
#define RDFT_4_OUTPUT_INDEX(i) (o_indexes_12__[(i)])
#include "Dft_B4.h"
#define RDFT_4_LOAD_RI_0(n, x_r, x_i) RDFT_LOAD_ARRAY_RI_0(&z_12__[4], x_r, x_i)
#define RDFT_4_LOAD_RI(i, x_r, x_i) RDFT_LOAD_ARRAY_RI(&z_12__[4], i, x_r, x_i)
#define RDFT_4_STORE RDFT_12_STORE
#define RDFT_4_OUTPUT_INDEX(i) (o_indexes_12__[(i) + 4])
#include "Dft_B4.h"
#define RDFT_4_LOAD_RI_0(n, x_r, x_i) RDFT_LOAD_ARRAY_RI_0(&z_12__[8], x_r, x_i)
#define RDFT_4_LOAD_RI(i, x_r, x_i) RDFT_LOAD_ARRAY_RI(&z_12__[8], i, x_r, x_i)
#define RDFT_4_STORE RDFT_12_STORE
#define RDFT_4_OUTPUT_INDEX(i) (o_indexes_12__[(i) + 8])
#include "Dft_B4.h"
}

#undef RDFT_12_LOAD_RI_0_
#undef RDFT_12_LOAD_RI
#undef RDFT_12_LOAD_RI_
#undef RDFT_12_STORE
