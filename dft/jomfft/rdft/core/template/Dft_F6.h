#ifndef RDFT_6_LOAD
#define RDFT_6_LOAD RDFT_LOAD
#endif

#ifndef RDFT_6_STORE_RI_0_
#define RDFT_6_STORE_RI_0_ RDFT_STORE_RI_0_
#endif

#ifndef RDFT_6_STORE_RI_h_
#define RDFT_6_STORE_RI_h_ RDFT_STORE_RI_h_
#endif

#ifndef RDFT_6_STORE_RI
#define RDFT_6_STORE_RI RDFT_STORE_RI
#endif

#define RDFT_6_STORE_RI_(i, z_r, z_i)                                          \
  do {                                                                         \
    if (i > (6 / 2)) {                                                         \
      z_i = RV_NEGATE(z_i);                                                    \
      RDFT_6_STORE_RI(6 - i, z_r, z_i);                                        \
    } else {                                                                   \
      RDFT_6_STORE_RI(i, z_r, z_i);                                            \
    }                                                                          \
  } while (0)

{
  const int i_indexes_6__[] = {0, 3, 4, 1, 2, 5};
  const int o_indexes_6__[] = {0, 3, 2, 5, 4, 1};
  RV_VECTOR z_6__[6];
#define RDFT_2_LOAD RDFT_6_LOAD
#define RDFT_2_INPUT_INDEX(i) (i_indexes_6__[(i)])
#define RDFT_2_STORE_RI_0(n, x_r, x_i)                                         \
  RDFT_STORE_ARRAY_RI_0(&z_6__[0], x_r, x_i)
#include "Dft_F2.h"
#define RDFT_2_LOAD RDFT_6_LOAD
#define RDFT_2_INPUT_INDEX(i) (i_indexes_6__[(i) + 2])
#define RDFT_2_STORE_RI_0(n, x_r, x_i)                                         \
  RDFT_STORE_ARRAY_RI_0(&z_6__[2], x_r, x_i)
#include "Dft_F2.h"
#define RDFT_2_LOAD RDFT_6_LOAD
#define RDFT_2_INPUT_INDEX(i) (i_indexes_6__[(i) + 4])
#define RDFT_2_STORE_RI_0(n, x_r, x_i)                                         \
  RDFT_STORE_ARRAY_RI_0(&z_6__[4], x_r, x_i)
#include "Dft_F2.h"
#define RDFT_3_LOAD(i) z_6__[2 * (i)]
#define RDFT_3_STORE_RI_0_(n, x) RDFT_6_STORE_RI_0_(6, x)
#define RDFT_3_STORE_RI(i, x_r, x_i) RDFT_6_STORE_RI_(i, x_r, x_i)
#define RDFT_3_OUTPUT_INDEX(i) o_indexes_6__[2 * (i)]
#include "Dft_F3.h"
#define RDFT_3_LOAD(i) z_6__[2 * (i) + 1]
#define RDFT_3_STORE_RI_0_(n, x) RDFT_6_STORE_RI_h_(6, x)
#define RDFT_3_STORE_RI(i, x_r, x_i) RDFT_6_STORE_RI_(i, x_r, x_i)
#define RDFT_3_OUTPUT_INDEX(i) o_indexes_6__[2 * (i) + 1]
#include "Dft_F3.h"
}

#undef RDFT_6_LOAD
#undef RDFT_6_STORE_RI_0_
#undef RDFT_6_STORE_RI_h_
#undef RDFT_6_STORE_RI
#undef RDFT_6_STORE_RI_
