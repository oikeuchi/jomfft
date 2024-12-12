#include "SplitRadixUtil_B.h"

#ifndef RDFT_8_LOAD_RI_0
#define RDFT_8_LOAD_RI_0 RDFT_LOAD_RI_0
#endif

#ifndef RDFT_8_LOAD_RI
#define RDFT_8_LOAD_RI RDFT_LOAD_RI
#endif

#ifndef RDFT_8_STORE
#define RDFT_8_STORE RDFT_STORE
#endif

#ifndef RDFT_8_INPUT_INDEX
#define RDFT_8_INPUT_INDEX(i) (i)
#endif

#ifndef RDFT_8_OUTPUT_INDEX
#define RDFT_8_OUTPUT_INDEX(i) (i)
#endif

{
  RV_VECTOR t_8__[4];
  RV_VECTOR u_8__[2];
  RV_VECTOR v_8__[2];

#define RDFT_SPLITRADIX_LOAD_RI_0(n, x0, xh) RDFT_8_LOAD_RI_0(n, x0, xh)
#define RDFT_SPLITRADIX_LOAD_RI(i, x_r, x_i)                                   \
  RDFT_8_LOAD_RI(RDFT_8_INPUT_INDEX(i), x_r, x_i)

  RDFT_SPLITRADIX_SPLIT0(8, t_8__, u_8__, v_8__);

#undef RDFT_SPLITRADIX_LOAD_RI_0
#undef RDFT_SPLITRADIX_LOAD_RI

#define RDFT_4_OUTPUT_INDEX(i) RDFT_8_OUTPUT_INDEX(2 * (i))
#define RDFT_4_LOAD_RI_0(n, x0, xh) RDFT_LOAD_ARRAY_RI_0(t_8__, x0, xh)
#define RDFT_4_LOAD_RI(i, x_r, x_i) RDFT_LOAD_ARRAY_RI(t_8__, i, x_r, x_i)
#include "Dft_B4.h"

#define RDFT_2_OUTPUT_INDEX(i) RDFT_8_OUTPUT_INDEX(4 * (i) + 1)
#define RDFT_2_LOAD_RI_0(n, x0, xh) RDFT_LOAD_ARRAY_RI_0(u_8__, x0, xh)
#include "Dft_B2.h"

#define RDFT_2_OUTPUT_INDEX(i) RDFT_8_OUTPUT_INDEX((4 * (i) + 7) % 8)
#define RDFT_2_LOAD_RI_0(n, x0, xh) RDFT_LOAD_ARRAY_RI_0(v_8__, x0, xh)
#include "Dft_B2.h"
}

#undef RDFT_8_LOAD_RI_0
#undef RDFT_8_LOAD_RI
#undef RDFT_8_STORE
#undef RDFT_8_INPUT_INDEX
#undef RDFT_8_OUTPUT_INDEX
