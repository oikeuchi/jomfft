#include <jomfft/math/constants/SinCos_9.h>

#ifndef RDFT_9_LOAD
#define RDFT_9_LOAD RDFT_LOAD
#endif

#ifndef RDFT_9_STORE_RI_0_
#define RDFT_9_STORE_RI_0_ RDFT_STORE_RI_0_
#endif

#ifndef RDFT_9_STORE_RI
#define RDFT_9_STORE_RI RDFT_STORE_RI
#endif

#ifndef RDFT_9_INPUT_INDEX
#define RDFT_9_INPUT_INDEX(i) (i)
#endif

#ifndef RDFT_9_OUTPUT_INDEX
#define RDFT_9_OUTPUT_INDEX(i) (i)
#endif

#define RDFT_9_STORE_RI_(i, z_r, z_i)                                          \
  do {                                                                         \
    if (i > (9 / 2)) {                                                         \
      z_i = RV_NEGATE(z_i);                                                    \
      RDFT_9_STORE_RI(9 - i, z_r, z_i);                                        \
    } else {                                                                   \
      RDFT_9_STORE_RI(i, z_r, z_i);                                            \
    }                                                                          \
  } while (0)

{
  RV_VECTOR z_9__[9];
#define RDFT_3_LOAD RDFT_9_LOAD
#define RDFT_3_INPUT_INDEX(i) RDFT_9_INPUT_INDEX(3 * (i))
#define RDFT_3_STORE_RI_0_(n, x) z_9__[0] = x
#define RDFT_3_STORE_RI(i, x_r, x_i)                                           \
  RDFT_STORE_ARRAY_RI(&z_9__[0] - 1, i, x_r, x_i)
#include "Dft_F3.h"
#define RDFT_3_LOAD RDFT_9_LOAD
#define RDFT_3_INPUT_INDEX(i) RDFT_9_INPUT_INDEX(3 * (i) + 1)
#define RDFT_3_STORE_RI_0_(n, x) z_9__[3] = x
#define RDFT_3_STORE_RI(i, x_r, x_i)                                           \
  RDFT_STORE_ARRAY_RI(&z_9__[3] - 1, i, x_r, x_i)
#include "Dft_F3.h"
#define RDFT_3_LOAD RDFT_9_LOAD
#define RDFT_3_INPUT_INDEX(i) RDFT_9_INPUT_INDEX(3 * (i) + 2)
#define RDFT_3_STORE_RI_0_(n, x) z_9__[6] = x
#define RDFT_3_STORE_RI(i, x_r, x_i)                                           \
  RDFT_STORE_ARRAY_RI(&z_9__[6] - 1, i, x_r, x_i)
#include "Dft_F3.h"
  SV_C_MUL_RI_(&z_9__[4], RV_SCALAR(COS2PI_9), RV_SCALAR(SIN2PI_9));
  SV_C_MUL_RI_(&z_9__[7], RV_SCALAR(COS4PI_9), RV_SCALAR(SIN4PI_9));
#define RDFT_3_LOAD(i) z_9__[3 * (i)]
#define RDFT_3_OUTPUT_INDEX(i) RDFT_9_OUTPUT_INDEX(3 * (i))
#define RDFT_3_STORE_RI_0_(n, x) RDFT_9_STORE_RI_0_(9, x)
#define RDFT_3_STORE_RI RDFT_9_STORE_RI
#include "Dft_F3.h"
#define SDFT_3_LOAD(i, x)                                                      \
  do {                                                                         \
    x[0] = z_9__[3 * (i) + 1];                                                 \
    x[1] = z_9__[3 * (i) + 2];                                                 \
  } while (0)
#define SDFT_3_OUTPUT_INDEX(i) RDFT_9_OUTPUT_INDEX(3 * (i) + 1)
#define SDFT_3_STORE(i, x) RDFT_9_STORE_RI_(i, x[0], x[1])
#include "SplitDft_3.h"
}

#undef RDFT_9_LOAD
#undef RDFT_9_STORE_RI_0_
#undef RDFT_9_STORE_RI
#undef RDFT_9_STORE_RI_
#undef RDFT_9_INPUT_INDEX
#undef RDFT_9_OUTPUT_INDEX
