#include <jomfft/math/constants/SinCos_16.h>

#include "SplitRadixUtil.h"

#ifndef CDFT_16_LOAD
#define CDFT_16_LOAD CDFT_LOAD
#endif

#ifndef CDFT_16_STORE
#define CDFT_16_STORE CDFT_STORE
#endif

#ifndef CDFT_16_INPUT_INDEX
#define CDFT_16_INPUT_INDEX(i) (i)
#endif

#ifndef CDFT_16_OUTPUT_INDEX
#define CDFT_16_OUTPUT_INDEX(i) (i)
#endif

{
  CV_VECTOR t_16__[8];
  CV_VECTOR u_16__[4];
  CV_VECTOR v_16__[4];
#define CDFT_4_INPUT_INDEX(i) CDFT_16_INPUT_INDEX((4 * (i) + 15) % 16)
#define CDFT_4_STORE(i, x) v_16__[i] = x
#include "Dft_4.h"

#define CDFT_4_INPUT_INDEX(i) CDFT_16_INPUT_INDEX(4 * (i) + 1)
#define CDFT_4_STORE(i, x) u_16__[i] = x
#include "Dft_4.h"

#define CDFT_8_INPUT_INDEX(i) CDFT_16_INPUT_INDEX(2 * (i))
#define CDFT_8_STORE(i, x) t_16__[i] = x
#include "Dft_8.h"

#define CDFT_SPLITRADIX_STORE(i, x) CDFT_16_STORE(CDFT_16_OUTPUT_INDEX(i), x)

  CDFT_SPLITRADIX_MERGE0(16, t_16__, u_16__, v_16__);
  CDFT_SPLITRADIX_MERGE(16, 1, COS2PI_16, SIN2PI_16, t_16__, u_16__, v_16__);
  CDFT_SPLITRADIX_MERGE_8(16, t_16__, u_16__, v_16__);

#undef CDFT_SPLITRADIX_STORE
}

#undef CDFT_16_LOAD
#undef CDFT_16_STORE
#undef CDFT_16_INPUT_INDEX
#undef CDFT_16_OUTPUT_INDEX
