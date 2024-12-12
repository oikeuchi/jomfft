#include "SplitRadixUtil_B.h"
#include <jomfft/math/constants/SinCos_32.h>

#ifndef RDFT_32_LOAD_RI_0
#define RDFT_32_LOAD_RI_0 RDFT_LOAD_RI_0
#endif

#ifndef RDFT_32_LOAD_RI
#define RDFT_32_LOAD_RI RDFT_LOAD_RI
#endif

#ifndef RDFT_32_STORE
#define RDFT_32_STORE RDFT_STORE
#endif

#ifndef RDFT_32_INPUT_INDEX
#define RDFT_32_INPUT_INDEX(i) (i)
#endif

#ifndef RDFT_32_OUTPUT_INDEX
#define RDFT_32_OUTPUT_INDEX(i) (i)
#endif

{
  RV_VECTOR t_32__[16];
  RV_VECTOR u_32__[8];
  RV_VECTOR v_32__[8];

#define RDFT_SPLITRADIX_LOAD_RI_0(n, x0, xh) RDFT_32_LOAD_RI_0(n, x0, xh)
#define RDFT_SPLITRADIX_LOAD_RI(i, x_r, x_i)                                   \
  RDFT_32_LOAD_RI(RDFT_32_INPUT_INDEX(i), x_r, x_i)

  RDFT_SPLITRADIX_SPLIT0(32, t_32__, u_32__, v_32__);
  RDFT_SPLITRADIX_SPLIT(32, 1, COS2PI_32, SIN2PI_32, t_32__, u_32__, v_32__);
  RDFT_SPLITRADIX_SPLIT(32, 2, COS4PI_32, SIN4PI_32, t_32__, u_32__, v_32__);
  RDFT_SPLITRADIX_SPLIT(32, 3, COS6PI_32, SIN6PI_32, t_32__, u_32__, v_32__);

#undef RDFT_SPLITRADIX_LOAD_RI_0
#undef RDFT_SPLITRADIX_LOAD_RI

#define RDFT_16_OUTPUT_INDEX(i) RDFT_32_OUTPUT_INDEX(2 * (i))
#define RDFT_16_LOAD_RI_0(n, x0, xh) RDFT_LOAD_ARRAY_RI_0(t_32__, x0, xh)
#define RDFT_16_LOAD_RI(i, x_r, x_i) RDFT_LOAD_ARRAY_RI(t_32__, i, x_r, x_i)
#include "Dft_B16.h"

#define RDFT_8_OUTPUT_INDEX(i) RDFT_32_OUTPUT_INDEX(4 * (i) + 1)
#define RDFT_8_LOAD_RI_0(n, x0, xh) RDFT_LOAD_ARRAY_RI_0(u_32__, x0, xh)
#define RDFT_8_LOAD_RI(i, x_r, x_i) RDFT_LOAD_ARRAY_RI(u_32__, i, x_r, x_i)
#include "Dft_B8.h"

#define RDFT_8_OUTPUT_INDEX(i) RDFT_32_OUTPUT_INDEX((4 * (i) + 31) % 32)
#define RDFT_8_LOAD_RI_0(n, x0, xh) RDFT_LOAD_ARRAY_RI_0(v_32__, x0, xh)
#define RDFT_8_LOAD_RI(i, x_r, x_i) RDFT_LOAD_ARRAY_RI(v_32__, i, x_r, x_i)
#include "Dft_B8.h"
}

#undef RDFT_32_LOAD_RI_0
#undef RDFT_32_LOAD_RI
#undef RDFT_32_STORE
#undef RDFT_32_INPUT_INDEX
#undef RDFT_32_OUTPUT_INDEX
