#ifndef RDFT_10_LOAD
#define RDFT_10_LOAD RDFT_LOAD
#endif

#ifndef RDFT_10_STORE_RI_0_
#define RDFT_10_STORE_RI_0_ RDFT_STORE_RI_0_
#endif

#ifndef RDFT_10_STORE_RI_h_
#define RDFT_10_STORE_RI_h_ RDFT_STORE_RI_h_
#endif

#ifndef RDFT_10_STORE_RI
#define RDFT_10_STORE_RI RDFT_STORE_RI
#endif

#define RDFT_10_STORE_RI_(i, z_r, z_i)                                         \
  do {                                                                         \
    if (i > (10 / 2)) {                                                        \
      z_i = RV_NEGATE(z_i);                                                    \
      RDFT_10_STORE_RI(10 - i, z_r, z_i);                                      \
    } else {                                                                   \
      RDFT_10_STORE_RI(i, z_r, z_i);                                           \
    }                                                                          \
  } while (0)

{
  const int i_indexes_10__[] = {0, 5, 6, 1, 2, 7, 8, 3, 4, 9};
  const int o_indexes_10__[] = {0, 5, 2, 7, 4, 9, 6, 1, 8, 3};
  RV_VECTOR z_10__[10];
#define RDFT_2_LOAD RDFT_10_LOAD
#define RDFT_2_INPUT_INDEX(i) (i_indexes_10__[(i)])
#define RDFT_2_STORE_RI_0(n, x_r, x_i)                                         \
  RDFT_STORE_ARRAY_RI_0(&z_10__[0], x_r, x_i)
#include "Dft_F2.h"
#define RDFT_2_LOAD RDFT_10_LOAD
#define RDFT_2_INPUT_INDEX(i) (i_indexes_10__[(i) + 2])
#define RDFT_2_STORE_RI_0(n, x_r, x_i)                                         \
  RDFT_STORE_ARRAY_RI_0(&z_10__[2], x_r, x_i)
#include "Dft_F2.h"
#define RDFT_2_LOAD RDFT_10_LOAD
#define RDFT_2_INPUT_INDEX(i) (i_indexes_10__[(i) + 4])
#define RDFT_2_STORE_RI_0(n, x_r, x_i)                                         \
  RDFT_STORE_ARRAY_RI_0(&z_10__[4], x_r, x_i)
#include "Dft_F2.h"
#define RDFT_2_LOAD RDFT_10_LOAD
#define RDFT_2_INPUT_INDEX(i) (i_indexes_10__[(i) + 6])
#define RDFT_2_STORE_RI_0(n, x_r, x_i)                                         \
  RDFT_STORE_ARRAY_RI_0(&z_10__[6], x_r, x_i)
#include "Dft_F2.h"
#define RDFT_2_LOAD RDFT_10_LOAD
#define RDFT_2_INPUT_INDEX(i) (i_indexes_10__[(i) + 8])
#define RDFT_2_STORE_RI_0(n, x_r, x_i)                                         \
  RDFT_STORE_ARRAY_RI_0(&z_10__[8], x_r, x_i)
#include "Dft_F2.h"
#define RDFT_5_LOAD(i) z_10__[2 * (i)]
#define RDFT_5_STORE_RI_0_(n, x) RDFT_10_STORE_RI_0_(10, x)
#define RDFT_5_STORE_RI(i, x_r, x_i) RDFT_10_STORE_RI_(i, x_r, x_i)
#define RDFT_5_OUTPUT_INDEX(i) o_indexes_10__[2 * (i)]
#include "Dft_F5.h"
#define RDFT_5_LOAD(i) z_10__[2 * (i) + 1]
#define RDFT_5_STORE_RI_0_(n, x) RDFT_10_STORE_RI_h_(10, x)
#define RDFT_5_STORE_RI(i, x_r, x_i) RDFT_10_STORE_RI_(i, x_r, x_i)
#define RDFT_5_OUTPUT_INDEX(i) o_indexes_10__[2 * (i) + 1]
#include "Dft_F5.h"
}

#undef RDFT_10_LOAD
#undef RDFT_10_STORE_RI_0_
#undef RDFT_10_STORE_RI_h_
#undef RDFT_10_STORE_RI
#undef RDFT_10_STORE_RI_
