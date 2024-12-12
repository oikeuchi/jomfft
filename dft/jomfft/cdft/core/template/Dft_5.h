#include <jomfft/math/constants/SinCos_5.h>

#ifndef CDFT_5_LOAD
#define CDFT_5_LOAD CDFT_LOAD
#endif

#ifndef CDFT_5_STORE
#define CDFT_5_STORE CDFT_STORE
#endif

#ifndef CDFT_5_INPUT_INDEX
#define CDFT_5_INPUT_INDEX(i) (i)
#endif

#ifndef CDFT_5_OUTPUT_INDEX
#define CDFT_5_OUTPUT_INDEX(i) (i)
#endif

{
  CV_VECTOR c[2];
  CV_VECTOR s[2];
  {
    CV_VECTOR u[2];
    CV_VECTOR v[2];
    CDFT_5_LOAD(CDFT_5_INPUT_INDEX(1), &u[0]);
    CDFT_5_LOAD(CDFT_5_INPUT_INDEX(4), &v[0]);
    CV_DFT2(&u[0], &v[0]);
    CDFT_5_LOAD(CDFT_5_INPUT_INDEX(2), &u[1]);
    CDFT_5_LOAD(CDFT_5_INPUT_INDEX(3), &v[1]);
    CV_DFT2(&u[1], &v[1]);
    {
      CV_VECTOR x0;
      CDFT_5_LOAD(CDFT_5_INPUT_INDEX(0), &x0);
      CV_DFT2(&u[0], &u[1]);
      CV_VECTOR z0 = CV_ADD(x0, u[0]);
      CDFT_5_STORE(CDFT_5_OUTPUT_INDEX(0), z0);
      u[0] = RV_FNMADD(RV_SCALAR(0.25), u[0], x0);
      u[1] = RV_MUL(RV_SCALAR(0.25 * SQRT5), u[1]);
      c[0] = CV_ADD(u[0], u[1]);
      c[1] = CV_SUB(u[0], u[1]);
    }
    {
      const CV_VECTOR sin4pi_5 = RV_SCALAR(SIN4PI_5);
      const CV_VECTOR sin2pi_5 = RV_SCALAR(SIN2PI_5);
      s[0] = RV_FMADD(sin2pi_5, v[0], RV_MUL(sin4pi_5, v[1]));
      s[1] = RV_FNMADD(sin2pi_5, v[1], RV_MUL(sin4pi_5, v[0]));
    }
  }
  CV_C_MULI_DFT2(&c[0], &s[0]);
  CDFT_5_STORE(CDFT_5_OUTPUT_INDEX(1), c[0]);
  CDFT_5_STORE(CDFT_5_OUTPUT_INDEX(4), s[0]);
  CV_C_MULI_DFT2(&c[1], &s[1]);
  CDFT_5_STORE(CDFT_5_OUTPUT_INDEX(2), c[1]);
  CDFT_5_STORE(CDFT_5_OUTPUT_INDEX(3), s[1]);
}

#undef CDFT_5_LOAD
#undef CDFT_5_STORE
#undef CDFT_5_INPUT_INDEX
#undef CDFT_5_OUTPUT_INDEX
