#include <jomfft/math/constants/SinCos_64.h>

#include "SplitRadixUtil.h"

#ifndef CDFT_64_LOAD
#define CDFT_64_LOAD CDFT_LOAD
#endif

#ifndef CDFT_64_STORE
#define CDFT_64_STORE CDFT_STORE
#endif

#ifndef CDFT_64_INPUT_INDEX
#define CDFT_64_INPUT_INDEX(i) (i)
#endif

#ifndef CDFT_64_OUTPUT_INDEX
#define CDFT_64_OUTPUT_INDEX(i) (i)
#endif

{
  CV_VECTOR t_64__[32];
  CV_VECTOR u_64__[16];
  CV_VECTOR v_64__[16];
#define CDFT_16_INPUT_INDEX(i) CDFT_64_INPUT_INDEX((4 * (i) + 63) % 64)
#define CDFT_16_STORE(i, x) v_64__[i] = x
#include "Dft_16.h"

#define CDFT_16_INPUT_INDEX(i) CDFT_64_INPUT_INDEX(4 * (i) + 1)
#define CDFT_16_STORE(i, x) u_64__[i] = x
#include "Dft_16.h"

#define CDFT_32_INPUT_INDEX(i) CDFT_64_INPUT_INDEX(2 * (i))
#define CDFT_32_STORE(i, x) t_64__[i] = x
#include "Dft_32.h"

#define CDFT_SPLITRADIX_STORE(i, x) CDFT_64_STORE(CDFT_64_OUTPUT_INDEX(i), x)

  CDFT_SPLITRADIX_MERGE0(64, t_64__, u_64__, v_64__);
  CDFT_SPLITRADIX_MERGE(64, 1, COS2PI_64, SIN2PI_64, t_64__, u_64__, v_64__);
  CDFT_SPLITRADIX_MERGE(64, 2, COS4PI_64, SIN4PI_64, t_64__, u_64__, v_64__);
  CDFT_SPLITRADIX_MERGE(64, 3, COS6PI_64, SIN6PI_64, t_64__, u_64__, v_64__);
  CDFT_SPLITRADIX_MERGE(64, 4, COS8PI_64, SIN8PI_64, t_64__, u_64__, v_64__);
  CDFT_SPLITRADIX_MERGE(64, 5, COS10PI_64, SIN10PI_64, t_64__, u_64__, v_64__);
  CDFT_SPLITRADIX_MERGE(64, 6, COS12PI_64, SIN12PI_64, t_64__, u_64__, v_64__);
  CDFT_SPLITRADIX_MERGE(64, 7, COS14PI_64, SIN14PI_64, t_64__, u_64__, v_64__);
  CDFT_SPLITRADIX_MERGE_8(64, t_64__, u_64__, v_64__);

#undef CDFT_SPLITRADIX_STORE
}

#undef CDFT_64_LOAD
#undef CDFT_64_STORE
#undef CDFT_64_INPUT_INDEX
#undef CDFT_64_OUTPUT_INDEX
