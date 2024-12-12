#include <jomfft/math/constants/SinCos_7.h>

#ifndef RDFT_7_LOAD
#define RDFT_7_LOAD RDFT_LOAD
#endif

#ifndef RDFT_7_STORE_RI_0_
#define RDFT_7_STORE_RI_0_ RDFT_STORE_RI_0_
#endif

#ifndef RDFT_7_STORE_RI
#define RDFT_7_STORE_RI RDFT_STORE_RI
#endif

#ifndef RDFT_7_INPUT_INDEX
#define RDFT_7_INPUT_INDEX(i) (i)
#endif

#ifndef RDFT_7_OUTPUT_INDEX
#define RDFT_7_OUTPUT_INDEX(i) (i)
#endif

{
  RV_VECTOR z0;
  RV_VECTOR c[3];
  RV_VECTOR s[3];
  {
    RV_VECTOR x0 = RDFT_7_LOAD(RDFT_7_INPUT_INDEX(0));
    RV_VECTOR u = RDFT_7_LOAD(RDFT_7_INPUT_INDEX(1));
    RV_VECTOR v = RDFT_7_LOAD(RDFT_7_INPUT_INDEX(6));
    RV_DFT2_CONJ(&u, &v);
    z0 = RV_ADD(x0, u);
    c[0] = RV_FMADD(RV_SCALAR(COS2PI_7), u, x0);
    c[1] = RV_FMADD(RV_SCALAR(COS4PI_7), u, x0);
    c[2] = RV_FMADD(RV_SCALAR(COS6PI_7), u, x0);
    s[0] = RV_MUL(RV_SCALAR(SIN2PI_7), v);
    s[1] = RV_MUL(RV_SCALAR(SIN4PI_7), v);
    s[2] = RV_MUL(RV_SCALAR(SIN6PI_7), v);
  }
  {
    RV_VECTOR u = RDFT_7_LOAD(RDFT_7_INPUT_INDEX(2));
    RV_VECTOR v = RDFT_7_LOAD(RDFT_7_INPUT_INDEX(5));
    RV_DFT2_CONJ(&u, &v);
    z0 = RV_ADD(z0, u);
    c[0] = RV_FMADD(RV_SCALAR(COS4PI_7), u, c[0]);
    c[1] = RV_FMADD(RV_SCALAR(COS6PI_7), u, c[1]);
    c[2] = RV_FMADD(RV_SCALAR(COS2PI_7), u, c[2]);
    s[0] = RV_FMADD(RV_SCALAR(SIN4PI_7), v, s[0]);
    s[1] = RV_FNMADD(RV_SCALAR(SIN6PI_7), v, s[1]);
    s[2] = RV_FNMADD(RV_SCALAR(SIN2PI_7), v, s[2]);
  }
  {
    RV_VECTOR u = RDFT_7_LOAD(RDFT_7_INPUT_INDEX(3));
    RV_VECTOR v = RDFT_7_LOAD(RDFT_7_INPUT_INDEX(4));
    RV_DFT2_CONJ(&u, &v);
    z0 = RV_ADD(z0, u);
    RDFT_7_STORE_RI_0_(7, z0);
    c[0] = RV_FMADD(RV_SCALAR(COS6PI_7), u, c[0]);
    c[1] = RV_FMADD(RV_SCALAR(COS2PI_7), u, c[1]);
    c[2] = RV_FMADD(RV_SCALAR(COS4PI_7), u, c[2]);
    s[0] = RV_FMADD(RV_SCALAR(SIN6PI_7), v, s[0]);
    s[1] = RV_FNMADD(RV_SCALAR(SIN2PI_7), v, s[1]);
    s[2] = RV_FMADD(RV_SCALAR(SIN4PI_7), v, s[2]);
  }
  RDFT_7_STORE_RI(RDFT_7_OUTPUT_INDEX(1), c[0], s[0]);
  RDFT_7_STORE_RI(RDFT_7_OUTPUT_INDEX(2), c[1], s[1]);
  RDFT_7_STORE_RI(RDFT_7_OUTPUT_INDEX(3), c[2], s[2]);
}

#undef RDFT_7_LOAD
#undef RDFT_7_STORE_RI_0_
#undef RDFT_7_STORE_RI
#undef RDFT_7_INPUT_INDEX
#undef RDFT_7_OUTPUT_INDEX
