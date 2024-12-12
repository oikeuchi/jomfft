#include <jomfft/math/constants/SinCos_32.h>

#include "SplitRadixUtil.h"

#ifndef CDFT_32_LOAD
#define CDFT_32_LOAD CDFT_LOAD
#endif

#ifndef CDFT_32_STORE
#define CDFT_32_STORE CDFT_STORE
#endif

#ifndef CDFT_32_INPUT_INDEX
#define CDFT_32_INPUT_INDEX(i) (i)
#endif

#ifndef CDFT_32_OUTPUT_INDEX
#define CDFT_32_OUTPUT_INDEX(i) (i)
#endif

{
  CV_VECTOR t_32__[16];
  CV_VECTOR u_32__[8];
  CV_VECTOR v_32__[8];
#define CDFT_8_INPUT_INDEX(i) CDFT_32_INPUT_INDEX((4 * (i) + 31) % 32)
#define CDFT_8_STORE(i, x) v_32__[i] = x
#include "Dft_8.h"

#define CDFT_8_INPUT_INDEX(i) CDFT_32_INPUT_INDEX(4 * (i) + 1)
#define CDFT_8_STORE(i, x) u_32__[i] = x
#include "Dft_8.h"

#define CDFT_16_INPUT_INDEX(i) CDFT_32_INPUT_INDEX(2 * (i))
#define CDFT_16_STORE(i, x) t_32__[i] = x
#include "Dft_16.h"

#define CDFT_SPLITRADIX_STORE(i, x) CDFT_32_STORE(CDFT_32_OUTPUT_INDEX(i), x)

  CDFT_SPLITRADIX_MERGE0(32, t_32__, u_32__, v_32__);
  CDFT_SPLITRADIX_MERGE(32, 1, COS2PI_32, SIN2PI_32, t_32__, u_32__, v_32__);
  CDFT_SPLITRADIX_MERGE(32, 2, COS4PI_32, SIN4PI_32, t_32__, u_32__, v_32__);
  CDFT_SPLITRADIX_MERGE(32, 3, COS6PI_32, SIN6PI_32, t_32__, u_32__, v_32__);
  CDFT_SPLITRADIX_MERGE_8(32, t_32__, u_32__, v_32__);

#undef CDFT_SPLITRADIX_STORE
}

#undef CDFT_32_LOAD
#undef CDFT_32_STORE
#undef CDFT_32_INPUT_INDEX
#undef CDFT_32_OUTPUT_INDEX
