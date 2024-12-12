#include <jomfft/math/constants/SinCos_128.h>

#include "SplitRadixUtil.h"

#ifndef CDFT_128_LOAD
#define CDFT_128_LOAD CDFT_LOAD
#endif

#ifndef CDFT_128_STORE
#define CDFT_128_STORE CDFT_STORE
#endif

#ifndef CDFT_128_INPUT_INDEX
#define CDFT_128_INPUT_INDEX(i) (i)
#endif

#ifndef CDFT_128_OUTPUT_INDEX
#define CDFT_128_OUTPUT_INDEX(i) (i)
#endif

{
  CV_VECTOR t_128__[64];
  CV_VECTOR u_128__[32];
  CV_VECTOR v_128__[32];
#define CDFT_32_INPUT_INDEX(i) CDFT_128_INPUT_INDEX((4 * (i) + 127) % 128)
#define CDFT_32_STORE(i, x) v_128__[i] = x
#include "Dft_32.h"

#define CDFT_32_INPUT_INDEX(i) CDFT_128_INPUT_INDEX(4 * (i) + 1)
#define CDFT_32_STORE(i, x) u_128__[i] = x
#include "Dft_32.h"

#define CDFT_64_INPUT_INDEX(i) CDFT_128_INPUT_INDEX(2 * (i))
#define CDFT_64_STORE(i, x) t_128__[i] = x
#include "Dft_64.h"

#define CDFT_SPLITRADIX_STORE(i, x) CDFT_128_STORE(CDFT_128_OUTPUT_INDEX(i), x)

  CDFT_SPLITRADIX_MERGE0(128, t_128__, u_128__, v_128__);
  CDFT_SPLITRADIX_MERGE(128, 1, COS2PI_128, SIN2PI_128, t_128__, u_128__,
                        v_128__);
  CDFT_SPLITRADIX_MERGE(128, 2, COS4PI_128, SIN4PI_128, t_128__, u_128__,
                        v_128__);
  CDFT_SPLITRADIX_MERGE(128, 3, COS6PI_128, SIN6PI_128, t_128__, u_128__,
                        v_128__);
  CDFT_SPLITRADIX_MERGE(128, 4, COS8PI_128, SIN8PI_128, t_128__, u_128__,
                        v_128__);
  CDFT_SPLITRADIX_MERGE(128, 5, COS10PI_128, SIN10PI_128, t_128__, u_128__,
                        v_128__);
  CDFT_SPLITRADIX_MERGE(128, 6, COS12PI_128, SIN12PI_128, t_128__, u_128__,
                        v_128__);
  CDFT_SPLITRADIX_MERGE(128, 7, COS14PI_128, SIN14PI_128, t_128__, u_128__,
                        v_128__);
  CDFT_SPLITRADIX_MERGE(128, 8, COS16PI_128, SIN16PI_128, t_128__, u_128__,
                        v_128__);
  CDFT_SPLITRADIX_MERGE(128, 9, COS18PI_128, SIN18PI_128, t_128__, u_128__,
                        v_128__);
  CDFT_SPLITRADIX_MERGE(128, 10, COS20PI_128, SIN20PI_128, t_128__, u_128__,
                        v_128__);
  CDFT_SPLITRADIX_MERGE(128, 11, COS22PI_128, SIN22PI_128, t_128__, u_128__,
                        v_128__);
  CDFT_SPLITRADIX_MERGE(128, 12, COS24PI_128, SIN24PI_128, t_128__, u_128__,
                        v_128__);
  CDFT_SPLITRADIX_MERGE(128, 13, COS26PI_128, SIN26PI_128, t_128__, u_128__,
                        v_128__);
  CDFT_SPLITRADIX_MERGE(128, 14, COS28PI_128, SIN28PI_128, t_128__, u_128__,
                        v_128__);
  CDFT_SPLITRADIX_MERGE(128, 15, COS30PI_128, SIN30PI_128, t_128__, u_128__,
                        v_128__);
  CDFT_SPLITRADIX_MERGE_8(128, t_128__, u_128__, v_128__);

#undef CDFT_SPLITRADIX_STORE
}

#undef CDFT_128_LOAD
#undef CDFT_128_STORE
#undef CDFT_128_INPUT_INDEX
#undef CDFT_128_OUTPUT_INDEX
