#include <jomfft/math/constants/SinCos_3.h>

#ifndef RDFT_3_LOAD_RI_0_
#define RDFT_3_LOAD_RI_0_ RDFT_LOAD_RI_0_
#endif

#ifndef RDFT_3_LOAD_RI
#define RDFT_3_LOAD_RI RDFT_LOAD_RI
#endif

#ifndef RDFT_3_STORE
#define RDFT_3_STORE RDFT_STORE
#endif

#ifndef RDFT_3_INPUT_INDEX
#define RDFT_3_INPUT_INDEX(i) (i)
#endif

#ifndef RDFT_3_OUTPUT_INDEX
#define RDFT_3_OUTPUT_INDEX(i) (i)
#endif

{
  RV_VECTOR x0 = RDFT_3_LOAD_RI_0_(3);
  RV_VECTOR u, v;
  RDFT_3_LOAD_RI(RDFT_3_INPUT_INDEX(1), &u, &v);
  u = RV_ADD(u, u);
  v = RV_ADD(v, v);
  RDFT_3_STORE(RDFT_3_OUTPUT_INDEX(0), RV_ADD(x0, u));
  u = RV_FMADD(RV_SCALAR(COS2PI_3), u, x0);
  v = RV_MUL(RV_SCALAR(SIN2PI_3), v);
  RV_CONJ_DFT2(&u, &v);
  RDFT_3_STORE(RDFT_3_OUTPUT_INDEX(1), u);
  RDFT_3_STORE(RDFT_3_OUTPUT_INDEX(2), v);
}

#undef RDFT_3_LOAD_RI_0_
#undef RDFT_3_LOAD_RI
#undef RDFT_3_STORE
#undef RDFT_3_INPUT_INDEX
#undef RDFT_3_OUTPUT_INDEX
