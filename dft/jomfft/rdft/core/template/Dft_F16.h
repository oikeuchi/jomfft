#include "SplitRadixUtil_F.h"
#include <jomfft/math/constants/SinCos_16.h>

#ifndef RDFT_16_LOAD
#define RDFT_16_LOAD RDFT_LOAD
#endif

#ifndef RDFT_16_STORE_RI_0
#define RDFT_16_STORE_RI_0 RDFT_STORE_RI_0
#endif

#ifndef RDFT_16_STORE_RI
#define RDFT_16_STORE_RI RDFT_STORE_RI
#endif

#ifndef RDFT_16_INPUT_INDEX
#define RDFT_16_INPUT_INDEX(i) (i)
#endif

#ifndef RDFT_16_OUTPUT_INDEX
#define RDFT_16_OUTPUT_INDEX(i) (i)
#endif

{
  RV_VECTOR t_16__[8];
  RV_VECTOR u_16__[4];
  RV_VECTOR v_16__[4];
#define RDFT_4_INPUT_INDEX(i) RDFT_16_INPUT_INDEX((4 * (i) + 15) % 16)
#define RDFT_4_STORE_RI_0(n, x0, xh) RDFT_STORE_ARRAY_RI_0(v_16__, x0, xh)
#define RDFT_4_STORE_RI(i, x_r, x_i) RDFT_STORE_ARRAY_RI(v_16__, i, x_r, x_i)
#include "Dft_F4.h"

#define RDFT_4_INPUT_INDEX(i) RDFT_16_INPUT_INDEX(4 * (i) + 1)
#define RDFT_4_STORE_RI_0(n, x0, xh) RDFT_STORE_ARRAY_RI_0(u_16__, x0, xh)
#define RDFT_4_STORE_RI(i, x_r, x_i) RDFT_STORE_ARRAY_RI(u_16__, i, x_r, x_i)
#include "Dft_F4.h"

#define RDFT_8_INPUT_INDEX(i) RDFT_16_INPUT_INDEX(2 * (i))
#define RDFT_8_STORE_RI_0(n, x0, xh) RDFT_STORE_ARRAY_RI_0(t_16__, x0, xh)
#define RDFT_8_STORE_RI(i, x_r, x_i) RDFT_STORE_ARRAY_RI(t_16__, i, x_r, x_i)
#include "Dft_F8.h"

#define RDFT_SPLITRADIX_STORE_RI_0(n, x0, xh) RDFT_16_STORE_RI_0(n, x0, xh)
#define RDFT_SPLITRADIX_STORE_RI(i, x_r, x_i)                                  \
  RDFT_16_STORE_RI(RDFT_16_OUTPUT_INDEX(i), x_r, x_i)

  RDFT_SPLITRADIX_MERGE0(16, t_16__, u_16__, v_16__);
  RDFT_SPLITRADIX_MERGE(16, 1, COS2PI_16, SIN2PI_16, t_16__, u_16__, v_16__);

#undef RDFT_SPLITRADIX_STORE_RI_0
#undef RDFT_SPLITRADIX_STORE_RI
}

#undef RDFT_16_LOAD
#undef RDFT_16_STORE_RI_0
#undef RDFT_16_STORE_RI
#undef RDFT_16_INPUT_INDEX
#undef RDFT_16_OUTPUT_INDEX
