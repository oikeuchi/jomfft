#include <jomfft/math/constants/SinCos_3.h>

#ifndef CDFT_3_LOAD
#define CDFT_3_LOAD CDFT_LOAD
#endif

#ifndef CDFT_3_STORE
#define CDFT_3_STORE CDFT_STORE
#endif

#ifndef CDFT_3_INPUT_INDEX
#define CDFT_3_INPUT_INDEX(i) (i)
#endif

#ifndef CDFT_3_OUTPUT_INDEX
#define CDFT_3_OUTPUT_INDEX(i) (i)
#endif

{
  CV_VECTOR x0, u, v;
  CDFT_3_LOAD(CDFT_3_INPUT_INDEX(0), &x0);
  CDFT_3_LOAD(CDFT_3_INPUT_INDEX(1), &u);
  CDFT_3_LOAD(CDFT_3_INPUT_INDEX(2), &v);
  CV_DFT2(&u, &v);
  CDFT_3_STORE(CDFT_3_OUTPUT_INDEX(0), CV_ADD(x0, u));
  u = RV_FMADD(RV_SCALAR(COS2PI_3), u, x0);
  v = RV_MUL(RV_SCALAR(SIN2PI_3), v);
  CV_C_MULI_DFT2(&u, &v);
  CDFT_3_STORE(CDFT_3_OUTPUT_INDEX(1), u);
  CDFT_3_STORE(CDFT_3_OUTPUT_INDEX(2), v);
}

#undef CDFT_3_LOAD
#undef CDFT_3_STORE
#undef CDFT_3_INPUT_INDEX
#undef CDFT_3_OUTPUT_INDEX
