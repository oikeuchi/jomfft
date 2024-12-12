#include "SplitRadixUtil_F.h"

#ifndef RDFT_8_LOAD
#define RDFT_8_LOAD RDFT_LOAD
#endif

#ifndef RDFT_8_STORE_RI_0
#define RDFT_8_STORE_RI_0 RDFT_STORE_RI_0
#endif

#ifndef RDFT_8_STORE_RI
#define RDFT_8_STORE_RI RDFT_STORE_RI
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
#define RDFT_2_INPUT_INDEX(i) RDFT_8_INPUT_INDEX((4 * (i) + 7) % 8)
#define RDFT_2_STORE_RI_0(n, x0, xh) RDFT_STORE_ARRAY_RI_0(v_8__, x0, xh)
#include "Dft_F2.h"

#define RDFT_2_INPUT_INDEX(i) RDFT_8_INPUT_INDEX(4 * (i) + 1)
#define RDFT_2_STORE_RI_0(n, x0, xh) RDFT_STORE_ARRAY_RI_0(u_8__, x0, xh)
#include "Dft_F2.h"

#define RDFT_4_INPUT_INDEX(i) RDFT_8_INPUT_INDEX(2 * (i))
#define RDFT_4_STORE_RI_0(n, x0, xh) RDFT_STORE_ARRAY_RI_0(t_8__, x0, xh)
#define RDFT_4_STORE_RI(i, x_r, x_i) RDFT_STORE_ARRAY_RI(t_8__, i, x_r, x_i)
#include "Dft_F4.h"

#define RDFT_SPLITRADIX_STORE_RI_0(n, x0, xh) RDFT_8_STORE_RI_0(n, x0, xh)
#define RDFT_SPLITRADIX_STORE_RI(i, x_r, x_i)                                  \
  RDFT_8_STORE_RI(RDFT_8_OUTPUT_INDEX(i), x_r, x_i)

  RDFT_SPLITRADIX_MERGE0(8, t_8__, u_8__, v_8__);

#undef RDFT_SPLITRADIX_STORE_RI_0
#undef RDFT_SPLITRADIX_STORE_RI
}

#undef RDFT_8_LOAD
#undef RDFT_8_STORE_RI_0
#undef RDFT_8_STORE_RI
#undef RDFT_8_INPUT_INDEX
#undef RDFT_8_OUTPUT_INDEX
