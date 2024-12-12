#include <jomfft/math/constants/SinCos_9.h>

#ifndef RDFT_6_LOAD_RI_0_
#define RDFT_6_LOAD_RI_0_ RDFT_LOAD_RI_0_
#endif

#ifndef RDFT_6_LOAD_RI_h_
#define RDFT_6_LOAD_RI_h_ RDFT_LOAD_RI_h_
#endif

#ifndef RDFT_6_LOAD_RI
#define RDFT_6_LOAD_RI RDFT_LOAD_RI
#endif

#ifndef RDFT_6_STORE
#define RDFT_6_STORE RDFT_STORE
#endif

#define RDFT_6_LOAD_RI_(i, z_r, z_i)                                           \
  do {                                                                         \
    if (i > (6 / 2)) {                                                         \
      RDFT_LOAD_RI(6 - i, z_r, z_i);                                           \
      *z_i = RV_NEGATE(*z_i);                                                  \
    } else {                                                                   \
      RDFT_LOAD_RI(i, z_r, z_i);                                               \
    }                                                                          \
  } while (0)

{
  const int i_indexes_6__[] = {0, 3, 2, 5, 4, 1};
  const int o_indexes_6__[] = {0, 3, 4, 1, 2, 5};
  RV_VECTOR z_6__[6];
#define RDFT_3_LOAD_RI_0_(n) RDFT_6_LOAD_RI_0_(6)
#define RDFT_3_LOAD_RI(i, x_r, x_i) RDFT_6_LOAD_RI_(i, x_r, x_i)
#define RDFT_3_INPUT_INDEX(i) i_indexes_6__[2 * (i)]
#define RDFT_3_STORE(i, x) z_6__[2 * (i)] = x
#include "Dft_B3.h"
#define RDFT_3_LOAD_RI_0_(n) RDFT_6_LOAD_RI_h_(6)
#define RDFT_3_LOAD_RI(i, x_r, x_i) RDFT_6_LOAD_RI_(i, x_r, x_i)
#define RDFT_3_INPUT_INDEX(i) i_indexes_6__[2 * (i) + 1]
#define RDFT_3_STORE(i, x) z_6__[2 * (i) + 1] = x
#include "Dft_B3.h"
#define RDFT_2_LOAD_RI_0(n, x_r, x_i) RDFT_LOAD_ARRAY_RI_0(&z_6__[0], x_r, x_i)
#define RDFT_2_STORE RDFT_6_STORE
#define RDFT_2_OUTPUT_INDEX(i) (o_indexes_6__[(i)])
#include "Dft_B2.h"
#define RDFT_2_LOAD_RI_0(n, x_r, x_i) RDFT_LOAD_ARRAY_RI_0(&z_6__[2], x_r, x_i)
#define RDFT_2_STORE RDFT_6_STORE
#define RDFT_2_OUTPUT_INDEX(i) (o_indexes_6__[(i) + 2])
#include "Dft_B2.h"
#define RDFT_2_LOAD_RI_0(n, x_r, x_i) RDFT_LOAD_ARRAY_RI_0(&z_6__[4], x_r, x_i)
#define RDFT_2_STORE RDFT_6_STORE
#define RDFT_2_OUTPUT_INDEX(i) (o_indexes_6__[(i) + 4])
#include "Dft_B2.h"
}

#undef RDFT_6_LOAD_RI_0_
#undef RDFT_6_LOAD_RI
#undef RDFT_6_LOAD_RI_
#undef RDFT_6_STORE
