#include <jomfft/math/constants/SinCos_9.h>

#ifndef RDFT_9_LOAD_RI_0_
#define RDFT_9_LOAD_RI_0_ RDFT_LOAD_RI_0_
#endif

#ifndef RDFT_9_LOAD_RI
#define RDFT_9_LOAD_RI RDFT_LOAD_RI
#endif

#ifndef RDFT_9_STORE
#define RDFT_9_STORE RDFT_STORE
#endif

#ifndef RDFT_9_INPUT_INDEX
#define RDFT_9_INPUT_INDEX(i) (i)
#endif

#ifndef RDFT_9_OUTPUT_INDEX
#define RDFT_9_OUTPUT_INDEX(i) (i)
#endif

#define RDFT_9_LOAD_RI_(i, z_r, z_i)                                           \
  do {                                                                         \
    if (i > (9 / 2)) {                                                         \
      RDFT_LOAD_RI(9 - i, z_r, z_i);                                           \
      *z_i = RV_NEGATE(*z_i);                                                  \
    } else {                                                                   \
      RDFT_LOAD_RI(i, z_r, z_i);                                               \
    }                                                                          \
  } while (0)

{
  RV_VECTOR z_9__[9];
#define RDFT_3_LOAD_RI_0_(n) RDFT_9_LOAD_RI_0_(9)
#define RDFT_3_LOAD_RI RDFT_9_LOAD_RI
#define RDFT_3_INPUT_INDEX(i) RDFT_9_INPUT_INDEX(3 * (i))
#define RDFT_3_STORE(i, x) z_9__[3 * (i)] = x
#include "Dft_B3.h"
#define SDFT_3_INPUT_INDEX(i) RDFT_9_INPUT_INDEX(3 * (i) + 1)
#define SDFT_3_LOAD(i, x) RDFT_9_LOAD_RI_(i, &x[0], &x[1])
#define SDFT_3_STORE(i, x)                                                     \
  do {                                                                         \
    z_9__[3 * (i) + 1] = x[0];                                                 \
    z_9__[3 * (i) + 2] = x[1];                                                 \
  } while (0)
#include "SplitDft_3.h"
  SV_C_MUL_RI_(&z_9__[4], RV_SCALAR(COS2PI_9), RV_SCALAR(SIN2PI_9));
  SV_C_MUL_RI_(&z_9__[7], RV_SCALAR(COS4PI_9), RV_SCALAR(SIN4PI_9));

#define RDFT_3_LOAD_RI_0_(n) z_9__[0]
#define RDFT_3_LOAD_RI(i, x_r, x_i)                                            \
  RDFT_LOAD_ARRAY_RI(&z_9__[0] - 1, i, x_r, x_i)
#define RDFT_3_STORE RDFT_9_STORE
#define RDFT_3_OUTPUT_INDEX(i) RDFT_9_OUTPUT_INDEX(3 * (i))
#include "Dft_B3.h"
#define RDFT_3_LOAD_RI_0_(n) z_9__[3]
#define RDFT_3_LOAD_RI(i, x_r, x_i)                                            \
  RDFT_LOAD_ARRAY_RI(&z_9__[3] - 1, i, x_r, x_i)
#define RDFT_3_STORE RDFT_9_STORE
#define RDFT_3_OUTPUT_INDEX(i) RDFT_9_OUTPUT_INDEX(3 * (i) + 1)
#include "Dft_B3.h"
#define RDFT_3_LOAD_RI_0_(n) z_9__[6]
#define RDFT_3_LOAD_RI(i, x_r, x_i)                                            \
  RDFT_LOAD_ARRAY_RI(&z_9__[6] - 1, i, x_r, x_i)
#define RDFT_3_STORE RDFT_9_STORE
#define RDFT_3_OUTPUT_INDEX(i) RDFT_9_OUTPUT_INDEX(3 * (i) + 2)
#include "Dft_B3.h"
}

#undef RDFT_9_LOAD_RI_0_
#undef RDFT_9_LOAD_RI
#undef RDFT_9_LOAD_RI_
#undef RDFT_9_STORE
#undef RDFT_9_INPUT_INDEX
#undef RDFT_9_OUTPUT_INDEX
