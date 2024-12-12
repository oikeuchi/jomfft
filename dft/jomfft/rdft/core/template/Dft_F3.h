#include <jomfft/math/constants/SinCos_3.h>

#ifndef RDFT_3_LOAD
#define RDFT_3_LOAD RDFT_LOAD
#endif

#ifndef RDFT_3_STORE_RI_0_
#define RDFT_3_STORE_RI_0_ RDFT_STORE_RI_0_
#endif

#ifndef RDFT_3_STORE_RI
#define RDFT_3_STORE_RI RDFT_STORE_RI
#endif

#ifndef RDFT_3_INPUT_INDEX
#define RDFT_3_INPUT_INDEX(i) (i)
#endif

#ifndef RDFT_3_OUTPUT_INDEX
#define RDFT_3_OUTPUT_INDEX(i) (i)
#endif

{
  RV_VECTOR x0 = RDFT_3_LOAD(RDFT_3_INPUT_INDEX(0));
  RV_VECTOR u = RDFT_3_LOAD(RDFT_3_INPUT_INDEX(1));
  RV_VECTOR v = RDFT_3_LOAD(RDFT_3_INPUT_INDEX(2));
  RV_DFT2_CONJ(&u, &v);
  RDFT_3_STORE_RI_0_(3, RV_ADD(x0, u));
  u = RV_FMADD(RV_SCALAR(COS2PI_3), u, x0);
  v = RV_MUL(RV_SCALAR(SIN2PI_3), v);
  RDFT_3_STORE_RI(RDFT_3_OUTPUT_INDEX(1), u, v);
}

#undef RDFT_3_LOAD
#undef RDFT_3_STORE_RI_0_
#undef RDFT_3_STORE_RI
#undef RDFT_3_INPUT_INDEX
#undef RDFT_3_OUTPUT_INDEX
