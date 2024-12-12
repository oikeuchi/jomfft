#include "SplitRadixUtil_B.h"
#include <jomfft/math/constants/SinCos_128.h>

#ifndef RDFT_128_LOAD_RI_0
#define RDFT_128_LOAD_RI_0 RDFT_LOAD_RI_0
#endif

#ifndef RDFT_128_LOAD_RI
#define RDFT_128_LOAD_RI RDFT_LOAD_RI
#endif

#ifndef RDFT_128_STORE
#define RDFT_128_STORE RDFT_STORE
#endif

#ifndef RDFT_128_INPUT_INDEX
#define RDFT_128_INPUT_INDEX(i) (i)
#endif

#ifndef RDFT_128_OUTPUT_INDEX
#define RDFT_128_OUTPUT_INDEX(i) (i)
#endif

{
  RV_VECTOR t_128__[64];
  RV_VECTOR u_128__[32];
  RV_VECTOR v_128__[32];

#define RDFT_SPLITRADIX_LOAD_RI_0(n, x0, xh) RDFT_128_LOAD_RI_0(n, x0, xh)
#define RDFT_SPLITRADIX_LOAD_RI(i, x_r, x_i)                                   \
  RDFT_128_LOAD_RI(RDFT_128_INPUT_INDEX(i), x_r, x_i)

  RDFT_SPLITRADIX_SPLIT0(128, t_128__, u_128__, v_128__);
  RDFT_SPLITRADIX_SPLIT(128, 1, COS2PI_128, SIN2PI_128, t_128__, u_128__,
                        v_128__);
  RDFT_SPLITRADIX_SPLIT(128, 2, COS4PI_128, SIN4PI_128, t_128__, u_128__,
                        v_128__);
  RDFT_SPLITRADIX_SPLIT(128, 3, COS6PI_128, SIN6PI_128, t_128__, u_128__,
                        v_128__);
  RDFT_SPLITRADIX_SPLIT(128, 4, COS8PI_128, SIN8PI_128, t_128__, u_128__,
                        v_128__);
  RDFT_SPLITRADIX_SPLIT(128, 5, COS10PI_128, SIN10PI_128, t_128__, u_128__,
                        v_128__);
  RDFT_SPLITRADIX_SPLIT(128, 6, COS12PI_128, SIN12PI_128, t_128__, u_128__,
                        v_128__);
  RDFT_SPLITRADIX_SPLIT(128, 7, COS14PI_128, SIN14PI_128, t_128__, u_128__,
                        v_128__);
  RDFT_SPLITRADIX_SPLIT(128, 8, COS16PI_128, SIN16PI_128, t_128__, u_128__,
                        v_128__);
  RDFT_SPLITRADIX_SPLIT(128, 9, COS18PI_128, SIN18PI_128, t_128__, u_128__,
                        v_128__);
  RDFT_SPLITRADIX_SPLIT(128, 10, COS20PI_128, SIN20PI_128, t_128__, u_128__,
                        v_128__);
  RDFT_SPLITRADIX_SPLIT(128, 11, COS22PI_128, SIN22PI_128, t_128__, u_128__,
                        v_128__);
  RDFT_SPLITRADIX_SPLIT(128, 12, COS24PI_128, SIN24PI_128, t_128__, u_128__,
                        v_128__);
  RDFT_SPLITRADIX_SPLIT(128, 13, COS26PI_128, SIN26PI_128, t_128__, u_128__,
                        v_128__);
  RDFT_SPLITRADIX_SPLIT(128, 14, COS28PI_128, SIN28PI_128, t_128__, u_128__,
                        v_128__);
  RDFT_SPLITRADIX_SPLIT(128, 15, COS30PI_128, SIN30PI_128, t_128__, u_128__,
                        v_128__);

#undef RDFT_SPLITRADIX_LOAD_RI_0
#undef RDFT_SPLITRADIX_LOAD_RI

#define RDFT_64_OUTPUT_INDEX(i) RDFT_128_OUTPUT_INDEX(2 * (i))
#define RDFT_64_LOAD_RI_0(n, x0, xh) RDFT_LOAD_ARRAY_RI_0(t_128__, x0, xh)
#define RDFT_64_LOAD_RI(i, x_r, x_i) RDFT_LOAD_ARRAY_RI(t_128__, i, x_r, x_i)
#include "Dft_B64.h"

#define RDFT_32_OUTPUT_INDEX(i) RDFT_128_OUTPUT_INDEX(4 * (i) + 1)
#define RDFT_32_LOAD_RI_0(n, x0, xh) RDFT_LOAD_ARRAY_RI_0(u_128__, x0, xh)
#define RDFT_32_LOAD_RI(i, x_r, x_i) RDFT_LOAD_ARRAY_RI(u_128__, i, x_r, x_i)
#include "Dft_B32.h"

#define RDFT_32_OUTPUT_INDEX(i) RDFT_128_OUTPUT_INDEX((4 * (i) + 127) % 128)
#define RDFT_32_LOAD_RI_0(n, x0, xh) RDFT_LOAD_ARRAY_RI_0(v_128__, x0, xh)
#define RDFT_32_LOAD_RI(i, x_r, x_i) RDFT_LOAD_ARRAY_RI(v_128__, i, x_r, x_i)
#include "Dft_B32.h"
}

#undef RDFT_128_LOAD_RI_0
#undef RDFT_128_LOAD_RI
#undef RDFT_128_STORE
#undef RDFT_128_INPUT_INDEX
#undef RDFT_128_OUTPUT_INDEX
