#include <jomfft/math/constants/SinCos_5.h>

#ifndef RDFT_5_LOAD_RI_0_
#define RDFT_5_LOAD_RI_0_ RDFT_LOAD_RI_0_
#endif

#ifndef RDFT_5_LOAD_RI
#define RDFT_5_LOAD_RI RDFT_LOAD_RI
#endif

#ifndef RDFT_5_STORE
#define RDFT_5_STORE RDFT_STORE
#endif

#ifndef RDFT_5_INPUT_INDEX
#define RDFT_5_INPUT_INDEX(i) (i)
#endif

#ifndef RDFT_5_OUTPUT_INDEX
#define RDFT_5_OUTPUT_INDEX(i) (i)
#endif

{
  RV_VECTOR c[2];
  RV_VECTOR s[2];
  {
    RV_VECTOR u[2];
    RV_VECTOR v[2];
    RDFT_5_LOAD_RI(RDFT_5_INPUT_INDEX(1), &u[0], &v[0]);
    u[0] = RV_ADD(u[0], u[0]);
    v[0] = RV_ADD(v[0], v[0]);
    RDFT_5_LOAD_RI(RDFT_5_INPUT_INDEX(2), &u[1], &v[1]);
    u[1] = RV_ADD(u[1], u[1]);
    v[1] = RV_ADD(v[1], v[1]);
    {
      RV_VECTOR x0 = RDFT_5_LOAD_RI_0_(5);
      RV_DFT2(&u[0], &u[1]);
      RV_VECTOR z0 = RV_ADD(x0, u[0]);
      RDFT_5_STORE(RDFT_5_OUTPUT_INDEX(0), z0);
      u[0] = RV_FNMADD(RV_SCALAR(0.25), u[0], x0);
      u[1] = RV_MUL(RV_SCALAR(0.25 * SQRT5), u[1]);
      c[0] = RV_ADD(u[0], u[1]);
      c[1] = RV_SUB(u[0], u[1]);
    }
    {
      const RV_VECTOR sin4pi_5 = RV_SCALAR(SIN4PI_5);
      const RV_VECTOR sin2pi_5 = RV_SCALAR(SIN2PI_5);
      s[0] = RV_FMADD(sin2pi_5, v[0], RV_MUL(sin4pi_5, v[1]));
      s[1] = RV_FNMADD(sin2pi_5, v[1], RV_MUL(sin4pi_5, v[0]));
    }
  }
  RV_CONJ_DFT2(&c[0], &s[0]);
  RDFT_5_STORE(RDFT_5_OUTPUT_INDEX(1), c[0]);
  RDFT_5_STORE(RDFT_5_OUTPUT_INDEX(4), s[0]);
  RV_CONJ_DFT2(&c[1], &s[1]);
  RDFT_5_STORE(RDFT_5_OUTPUT_INDEX(2), c[1]);
  RDFT_5_STORE(RDFT_5_OUTPUT_INDEX(3), s[1]);
}

#undef RDFT_5_LOAD_RI_0_
#undef RDFT_5_LOAD_RI
#undef RDFT_5_STORE
#undef RDFT_5_INPUT_INDEX
#undef RDFT_5_OUTPUT_INDEX
