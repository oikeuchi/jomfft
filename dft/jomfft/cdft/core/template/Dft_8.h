#include "SplitRadixUtil.h"

#ifndef CDFT_8_LOAD
#define CDFT_8_LOAD CDFT_LOAD
#endif

#ifndef CDFT_8_STORE
#define CDFT_8_STORE CDFT_STORE
#endif

#ifndef CDFT_8_INPUT_INDEX
#define CDFT_8_INPUT_INDEX(i) (i)
#endif

#ifndef CDFT_8_OUTPUT_INDEX
#define CDFT_8_OUTPUT_INDEX(i) (i)
#endif

{
  CV_VECTOR t_8__[4];
  CV_VECTOR u_8__[2];
  CV_VECTOR v_8__[2];
#define CDFT_2_INPUT_INDEX(i) CDFT_8_INPUT_INDEX((4 * (i) + 7) % 8)
#define CDFT_2_STORE(i, x) v_8__[i] = x
#include "Dft_2.h"

#define CDFT_2_INPUT_INDEX(i) CDFT_8_INPUT_INDEX(4 * (i) + 1)
#define CDFT_2_STORE(i, x) u_8__[i] = x
#include "Dft_2.h"

#define CDFT_4_INPUT_INDEX(i) CDFT_8_INPUT_INDEX(2 * (i))
#define CDFT_4_STORE(i, x) t_8__[i] = x
#include "Dft_4.h"

#define CDFT_SPLITRADIX_STORE(i, x) CDFT_8_STORE(CDFT_8_OUTPUT_INDEX(i), x)

  CDFT_SPLITRADIX_MERGE0(8, t_8__, u_8__, v_8__);
  CDFT_SPLITRADIX_MERGE_8(8, t_8__, u_8__, v_8__);

#undef CDFT_SPLITRADIX_STORE
}

#undef CDFT_8_LOAD
#undef CDFT_8_STORE
#undef CDFT_8_INPUT_INDEX
#undef CDFT_8_OUTPUT_INDEX
