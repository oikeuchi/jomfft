#include <jomfft/math/constants/SinCos_7.h>

#ifndef RDFT_7_LOAD_RI_0_
#define RDFT_7_LOAD_RI_0_ RDFT_LOAD_RI_0_
#endif

#ifndef RDFT_7_LOAD_RI
#define RDFT_7_LOAD_RI RDFT_LOAD_RI
#endif

#ifndef RDFT_7_STORE
#define RDFT_7_STORE RDFT_STORE
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
    RV_VECTOR x0 = RDFT_7_LOAD_RI_0_(7);
    RV_VECTOR u, v;
    RDFT_7_LOAD_RI(RDFT_7_INPUT_INDEX(1), &u, &v);
    u = RV_ADD(u, u);
    v = RV_ADD(v, v);
    z0 = RV_ADD(x0, u);
    c[0] = RV_FMADD(RV_SCALAR(COS2PI_7), u, x0);
    c[1] = RV_FMADD(RV_SCALAR(COS4PI_7), u, x0);
    c[2] = RV_FMADD(RV_SCALAR(COS6PI_7), u, x0);
    s[0] = RV_MUL(RV_SCALAR(SIN2PI_7), v);
    s[1] = RV_MUL(RV_SCALAR(SIN4PI_7), v);
    s[2] = RV_MUL(RV_SCALAR(SIN6PI_7), v);
  }
  {
    RV_VECTOR u, v;
    RDFT_7_LOAD_RI(RDFT_7_INPUT_INDEX(2), &u, &v);
    u = RV_ADD(u, u);
    v = RV_ADD(v, v);
    z0 = RV_ADD(z0, u);
    c[0] = RV_FMADD(RV_SCALAR(COS4PI_7), u, c[0]);
    c[1] = RV_FMADD(RV_SCALAR(COS6PI_7), u, c[1]);
    c[2] = RV_FMADD(RV_SCALAR(COS2PI_7), u, c[2]);
    s[0] = RV_FMADD(RV_SCALAR(SIN4PI_7), v, s[0]);
    s[1] = RV_FNMADD(RV_SCALAR(SIN6PI_7), v, s[1]);
    s[2] = RV_FNMADD(RV_SCALAR(SIN2PI_7), v, s[2]);
  }
  {
    RV_VECTOR u, v;
    RDFT_7_LOAD_RI(RDFT_7_INPUT_INDEX(3), &u, &v);
    u = RV_ADD(u, u);
    v = RV_ADD(v, v);
    z0 = RV_ADD(z0, u);
    RDFT_7_STORE(RDFT_7_OUTPUT_INDEX(0), z0);
    c[0] = RV_FMADD(RV_SCALAR(COS6PI_7), u, c[0]);
    c[1] = RV_FMADD(RV_SCALAR(COS2PI_7), u, c[1]);
    c[2] = RV_FMADD(RV_SCALAR(COS4PI_7), u, c[2]);
    s[0] = RV_FMADD(RV_SCALAR(SIN6PI_7), v, s[0]);
    s[1] = RV_FNMADD(RV_SCALAR(SIN2PI_7), v, s[1]);
    s[2] = RV_FMADD(RV_SCALAR(SIN4PI_7), v, s[2]);
  }
  RV_CONJ_DFT2(&c[0], &s[0]);
  RDFT_7_STORE(RDFT_7_OUTPUT_INDEX(1), c[0]);
  RDFT_7_STORE(RDFT_7_OUTPUT_INDEX(6), s[0]);
  RV_CONJ_DFT2(&c[1], &s[1]);
  RDFT_7_STORE(RDFT_7_OUTPUT_INDEX(2), c[1]);
  RDFT_7_STORE(RDFT_7_OUTPUT_INDEX(5), s[1]);
  RV_CONJ_DFT2(&c[2], &s[2]);
  RDFT_7_STORE(RDFT_7_OUTPUT_INDEX(3), c[2]);
  RDFT_7_STORE(RDFT_7_OUTPUT_INDEX(4), s[2]);
}

#undef RDFT_7_LOAD_RI_0_
#undef RDFT_7_LOAD_RI
#undef RDFT_7_STORE
#undef RDFT_7_INPUT_INDEX
#undef RDFT_7_OUTPUT_INDEX
