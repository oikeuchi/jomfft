#ifndef RDFT_12_LOAD
#define RDFT_12_LOAD RDFT_LOAD
#endif

#ifndef RDFT_12_STORE_RI_0_
#define RDFT_12_STORE_RI_0_ RDFT_STORE_RI_0_
#endif

#ifndef RDFT_12_STORE_RI_h_
#define RDFT_12_STORE_RI_h_ RDFT_STORE_RI_h_
#endif

#ifndef RDFT_12_STORE_RI
#define RDFT_12_STORE_RI RDFT_STORE_RI
#endif

#define RDFT_12_STORE_RI_(i, z_r, z_i)                                         \
  do {                                                                         \
    if (i > (12 / 2)) {                                                        \
      z_i = RV_NEGATE(z_i);                                                    \
      RDFT_12_STORE_RI(12 - i, z_r, z_i);                                      \
    } else {                                                                   \
      RDFT_12_STORE_RI(i, z_r, z_i);                                           \
    }                                                                          \
  } while (0)

{
  const int i_indexes_12__[] = {0, 9, 6, 3, 4, 1, 10, 7, 8, 5, 2, 11};
  const int o_indexes_12__[] = {0, 3, 6, 9, 4, 7, 10, 1, 8, 11, 2, 5};
  RV_VECTOR z_12__[12];
#define RDFT_4_LOAD RDFT_12_LOAD
#define RDFT_4_INPUT_INDEX(i) (i_indexes_12__[(i)])
#define RDFT_4_STORE_RI_0(n, x_r, x_i)                                         \
  RDFT_STORE_ARRAY_RI_0(&z_12__[0], x_r, x_i)
#define RDFT_4_STORE_RI(i, x_r, x_i)                                           \
  RDFT_STORE_ARRAY_RI(&z_12__[0], i, x_r, x_i)
#include "Dft_F4.h"
#define RDFT_4_LOAD RDFT_12_LOAD
#define RDFT_4_INPUT_INDEX(i) (i_indexes_12__[(i) + 4])
#define RDFT_4_STORE_RI_0(n, x_r, x_i)                                         \
  RDFT_STORE_ARRAY_RI_0(&z_12__[4], x_r, x_i)
#define RDFT_4_STORE_RI(i, x_r, x_i)                                           \
  RDFT_STORE_ARRAY_RI(&z_12__[4], i, x_r, x_i)
#include "Dft_F4.h"
#define RDFT_4_LOAD RDFT_12_LOAD
#define RDFT_4_INPUT_INDEX(i) (i_indexes_12__[(i) + 8])
#define RDFT_4_STORE_RI_0(n, x_r, x_i)                                         \
  RDFT_STORE_ARRAY_RI_0(&z_12__[8], x_r, x_i)
#define RDFT_4_STORE_RI(i, x_r, x_i)                                           \
  RDFT_STORE_ARRAY_RI(&z_12__[8], i, x_r, x_i)
#include "Dft_F4.h"
#define RDFT_3_LOAD(i) z_12__[4 * (i)]
#define RDFT_3_STORE_RI_0_ RDFT_12_STORE_RI_0_
#define RDFT_3_STORE_RI(i, x_r, x_i) RDFT_12_STORE_RI_(i, x_r, x_i)
#define RDFT_3_OUTPUT_INDEX(i) o_indexes_12__[4 * (i)]
#include "Dft_F3.h"
#define RDFT_3_LOAD(i) z_12__[4 * (i) + 1]
#define RDFT_3_STORE_RI_0_(n, x) RDFT_12_STORE_RI_h_(12, x)
#define RDFT_3_STORE_RI(i, x_r, x_i) RDFT_12_STORE_RI_(i, x_r, x_i)
#define RDFT_3_OUTPUT_INDEX(i) o_indexes_12__[4 * (i) + 2]
#include "Dft_F3.h"
#define SDFT_3_LOAD(i, x)                                                      \
  do {                                                                         \
    x[0] = z_12__[4 * (i) + 2];                                                \
    x[1] = z_12__[4 * (i) + 3];                                                \
  } while (0)
#define SDFT_3_OUTPUT_INDEX(i) o_indexes_12__[4 * (i) + 1]
#define SDFT_3_STORE(i, x) RDFT_12_STORE_RI_(i, x[0], x[1])
#include "SplitDft_3.h"
}

#undef RDFT_12_LOAD
#undef RDFT_12_STORE_RI_0_
#undef RDFT_12_STORE_RI_h_
#undef RDFT_12_STORE_RI
#undef RDFT_12_STORE_RI_
