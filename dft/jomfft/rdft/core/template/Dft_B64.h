#include "SplitRadixUtil_B.h"
#include <jomfft/math/constants/SinCos_64.h>

#ifndef RDFT_64_LOAD_RI_0
#define RDFT_64_LOAD_RI_0 RDFT_LOAD_RI_0
#endif

#ifndef RDFT_64_LOAD_RI
#define RDFT_64_LOAD_RI RDFT_LOAD_RI
#endif

#ifndef RDFT_64_STORE
#define RDFT_64_STORE RDFT_STORE
#endif

#ifndef RDFT_64_INPUT_INDEX
#define RDFT_64_INPUT_INDEX(i) (i)
#endif

#ifndef RDFT_64_OUTPUT_INDEX
#define RDFT_64_OUTPUT_INDEX(i) (i)
#endif

{
  RV_VECTOR t_64__[32];
  RV_VECTOR u_64__[16];
  RV_VECTOR v_64__[16];

#define RDFT_SPLITRADIX_LOAD_RI_0(n, x0, xh) RDFT_64_LOAD_RI_0(n, x0, xh)
#define RDFT_SPLITRADIX_LOAD_RI(i, x_r, x_i)                                   \
  RDFT_64_LOAD_RI(RDFT_64_INPUT_INDEX(i), x_r, x_i)

  RDFT_SPLITRADIX_SPLIT0(64, t_64__, u_64__, v_64__);
  RDFT_SPLITRADIX_SPLIT(64, 1, COS2PI_64, SIN2PI_64, t_64__, u_64__, v_64__);
  RDFT_SPLITRADIX_SPLIT(64, 2, COS4PI_64, SIN4PI_64, t_64__, u_64__, v_64__);
  RDFT_SPLITRADIX_SPLIT(64, 3, COS6PI_64, SIN6PI_64, t_64__, u_64__, v_64__);
  RDFT_SPLITRADIX_SPLIT(64, 4, COS8PI_64, SIN8PI_64, t_64__, u_64__, v_64__);
  RDFT_SPLITRADIX_SPLIT(64, 5, COS10PI_64, SIN10PI_64, t_64__, u_64__, v_64__);
  RDFT_SPLITRADIX_SPLIT(64, 6, COS12PI_64, SIN12PI_64, t_64__, u_64__, v_64__);
  RDFT_SPLITRADIX_SPLIT(64, 7, COS14PI_64, SIN14PI_64, t_64__, u_64__, v_64__);

#undef RDFT_SPLITRADIX_LOAD_RI_0
#undef RDFT_SPLITRADIX_LOAD_RI

#define RDFT_32_OUTPUT_INDEX(i) RDFT_64_OUTPUT_INDEX(2 * (i))
#define RDFT_32_LOAD_RI_0(n, x0, xh) RDFT_LOAD_ARRAY_RI_0(t_64__, x0, xh)
#define RDFT_32_LOAD_RI(i, x_r, x_i) RDFT_LOAD_ARRAY_RI(t_64__, i, x_r, x_i)
#include "Dft_B32.h"

#define RDFT_16_OUTPUT_INDEX(i) RDFT_64_OUTPUT_INDEX(4 * (i) + 1)
#define RDFT_16_LOAD_RI_0(n, x0, xh) RDFT_LOAD_ARRAY_RI_0(u_64__, x0, xh)
#define RDFT_16_LOAD_RI(i, x_r, x_i) RDFT_LOAD_ARRAY_RI(u_64__, i, x_r, x_i)
#include "Dft_B16.h"

#define RDFT_16_OUTPUT_INDEX(i) RDFT_64_OUTPUT_INDEX((4 * (i) + 63) % 64)
#define RDFT_16_LOAD_RI_0(n, x0, xh) RDFT_LOAD_ARRAY_RI_0(v_64__, x0, xh)
#define RDFT_16_LOAD_RI(i, x_r, x_i) RDFT_LOAD_ARRAY_RI(v_64__, i, x_r, x_i)
#include "Dft_B16.h"
}

#undef RDFT_64_LOAD_RI_0
#undef RDFT_64_LOAD_RI
#undef RDFT_64_STORE
#undef RDFT_64_INPUT_INDEX
#undef RDFT_64_OUTPUT_INDEX
