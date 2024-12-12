#include <jomfft/math/constants/SinCos_7.h>

#ifndef CDFT_7_LOAD
#define CDFT_7_LOAD CDFT_LOAD
#endif

#ifndef CDFT_7_STORE
#define CDFT_7_STORE CDFT_STORE
#endif

#ifndef CDFT_7_INPUT_INDEX
#define CDFT_7_INPUT_INDEX(i) (i)
#endif

#ifndef CDFT_7_OUTPUT_INDEX
#define CDFT_7_OUTPUT_INDEX(i) (i)
#endif

{
  CV_VECTOR z0;
  CV_VECTOR c[3];
  CV_VECTOR s[3];
  {
    CV_VECTOR x0, u, v;
    CDFT_7_LOAD(CDFT_7_INPUT_INDEX(0), &x0);
    CDFT_7_LOAD(CDFT_7_INPUT_INDEX(1), &u);
    CDFT_7_LOAD(CDFT_7_INPUT_INDEX(6), &v);
    CV_DFT2(&u, &v);
    z0 = CV_ADD(x0, u);
    c[0] = RV_FMADD(RV_SCALAR(COS2PI_7), u, x0);
    c[1] = RV_FMADD(RV_SCALAR(COS4PI_7), u, x0);
    c[2] = RV_FMADD(RV_SCALAR(COS6PI_7), u, x0);
    s[0] = RV_MUL(RV_SCALAR(SIN2PI_7), v);
    s[1] = RV_MUL(RV_SCALAR(SIN4PI_7), v);
    s[2] = RV_MUL(RV_SCALAR(SIN6PI_7), v);
  }
  {
    CV_VECTOR u, v;
    CDFT_7_LOAD(CDFT_7_INPUT_INDEX(2), &u);
    CDFT_7_LOAD(CDFT_7_INPUT_INDEX(5), &v);
    CV_DFT2(&u, &v);
    z0 = CV_ADD(z0, u);
    c[0] = RV_FMADD(RV_SCALAR(COS4PI_7), u, c[0]);
    c[1] = RV_FMADD(RV_SCALAR(COS6PI_7), u, c[1]);
    c[2] = RV_FMADD(RV_SCALAR(COS2PI_7), u, c[2]);
    s[0] = RV_FMADD(RV_SCALAR(SIN4PI_7), v, s[0]);
    s[1] = RV_FNMADD(RV_SCALAR(SIN6PI_7), v, s[1]);
    s[2] = RV_FNMADD(RV_SCALAR(SIN2PI_7), v, s[2]);
  }
  {
    CV_VECTOR u, v;
    CDFT_7_LOAD(CDFT_7_INPUT_INDEX(3), &u);
    CDFT_7_LOAD(CDFT_7_INPUT_INDEX(4), &v);
    CV_DFT2(&u, &v);
    z0 = CV_ADD(z0, u);
    CDFT_7_STORE(CDFT_7_OUTPUT_INDEX(0), z0);
    c[0] = RV_FMADD(RV_SCALAR(COS6PI_7), u, c[0]);
    c[1] = RV_FMADD(RV_SCALAR(COS2PI_7), u, c[1]);
    c[2] = RV_FMADD(RV_SCALAR(COS4PI_7), u, c[2]);
    s[0] = RV_FMADD(RV_SCALAR(SIN6PI_7), v, s[0]);
    s[1] = RV_FNMADD(RV_SCALAR(SIN2PI_7), v, s[1]);
    s[2] = RV_FMADD(RV_SCALAR(SIN4PI_7), v, s[2]);
  }
  CV_C_MULI_DFT2(&c[0], &s[0]);
  CDFT_7_STORE(CDFT_7_OUTPUT_INDEX(1), c[0]);
  CDFT_7_STORE(CDFT_7_OUTPUT_INDEX(6), s[0]);
  CV_C_MULI_DFT2(&c[1], &s[1]);
  CDFT_7_STORE(CDFT_7_OUTPUT_INDEX(2), c[1]);
  CDFT_7_STORE(CDFT_7_OUTPUT_INDEX(5), s[1]);
  CV_C_MULI_DFT2(&c[2], &s[2]);
  CDFT_7_STORE(CDFT_7_OUTPUT_INDEX(3), c[2]);
  CDFT_7_STORE(CDFT_7_OUTPUT_INDEX(4), s[2]);
}

#undef CDFT_7_LOAD
#undef CDFT_7_STORE
#undef CDFT_7_INPUT_INDEX
#undef CDFT_7_OUTPUT_INDEX
