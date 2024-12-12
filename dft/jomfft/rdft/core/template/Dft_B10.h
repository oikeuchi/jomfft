#include <jomfft/math/constants/SinCos_9.h>

#ifndef RDFT_10_LOAD_RI_0_
#define RDFT_10_LOAD_RI_0_ RDFT_LOAD_RI_0_
#endif

#ifndef RDFT_10_LOAD_RI_h_
#define RDFT_10_LOAD_RI_h_ RDFT_LOAD_RI_h_
#endif

#ifndef RDFT_10_LOAD_RI
#define RDFT_10_LOAD_RI RDFT_LOAD_RI
#endif

#ifndef RDFT_10_STORE
#define RDFT_10_STORE RDFT_STORE
#endif

#define RDFT_10_LOAD_RI_(i, z_r, z_i)                                          \
  do {                                                                         \
    if (i > (10 / 2)) {                                                        \
      RDFT_LOAD_RI(10 - i, z_r, z_i);                                          \
      *z_i = RV_NEGATE(*z_i);                                                  \
    } else {                                                                   \
      RDFT_LOAD_RI(i, z_r, z_i);                                               \
    }                                                                          \
  } while (0)

{
  const int i_indexes_10__[] = {0, 5, 2, 7, 4, 9, 6, 1, 8, 3};
  const int o_indexes_10__[] = {0, 5, 6, 1, 2, 7, 8, 3, 4, 9};
  RV_VECTOR z_10__[10];
#define RDFT_5_LOAD_RI_0_(n) RDFT_10_LOAD_RI_0_(10)
#define RDFT_5_LOAD_RI(i, x_r, x_i) RDFT_10_LOAD_RI_(i, x_r, x_i)
#define RDFT_5_INPUT_INDEX(i) i_indexes_10__[2 * (i)]
#define RDFT_5_STORE(i, x) z_10__[2 * (i)] = x
#include "Dft_B5.h"
#define RDFT_5_LOAD_RI_0_(n) RDFT_10_LOAD_RI_h_(10)
#define RDFT_5_LOAD_RI(i, x_r, x_i) RDFT_10_LOAD_RI_(i, x_r, x_i)
#define RDFT_5_INPUT_INDEX(i) i_indexes_10__[2 * (i) + 1]
#define RDFT_5_STORE(i, x) z_10__[2 * (i) + 1] = x
#include "Dft_B5.h"
#define RDFT_2_LOAD_RI_0(n, x_r, x_i) RDFT_LOAD_ARRAY_RI_0(&z_10__[0], x_r, x_i)
#define RDFT_2_STORE RDFT_10_STORE
#define RDFT_2_OUTPUT_INDEX(i) (o_indexes_10__[(i)])
#include "Dft_B2.h"
#define RDFT_2_LOAD_RI_0(n, x_r, x_i) RDFT_LOAD_ARRAY_RI_0(&z_10__[2], x_r, x_i)
#define RDFT_2_STORE RDFT_10_STORE
#define RDFT_2_OUTPUT_INDEX(i) (o_indexes_10__[(i) + 2])
#include "Dft_B2.h"
#define RDFT_2_LOAD_RI_0(n, x_r, x_i) RDFT_LOAD_ARRAY_RI_0(&z_10__[4], x_r, x_i)
#define RDFT_2_STORE RDFT_10_STORE
#define RDFT_2_OUTPUT_INDEX(i) (o_indexes_10__[(i) + 4])
#include "Dft_B2.h"
#define RDFT_2_LOAD_RI_0(n, x_r, x_i) RDFT_LOAD_ARRAY_RI_0(&z_10__[6], x_r, x_i)
#define RDFT_2_STORE RDFT_10_STORE
#define RDFT_2_OUTPUT_INDEX(i) (o_indexes_10__[(i) + 6])
#include "Dft_B2.h"
#define RDFT_2_LOAD_RI_0(n, x_r, x_i) RDFT_LOAD_ARRAY_RI_0(&z_10__[8], x_r, x_i)
#define RDFT_2_STORE RDFT_10_STORE
#define RDFT_2_OUTPUT_INDEX(i) (o_indexes_10__[(i) + 8])
#include "Dft_B2.h"
}

#undef RDFT_10_LOAD_RI_0_
#undef RDFT_10_LOAD_RI
#undef RDFT_10_LOAD_RI_
#undef RDFT_10_STORE
