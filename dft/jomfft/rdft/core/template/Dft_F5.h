#include <jomfft/math/constants/SinCos_5.h>

#ifndef RDFT_5_LOAD
#define RDFT_5_LOAD RDFT_LOAD
#endif

#ifndef RDFT_5_STORE_RI_0_
#define RDFT_5_STORE_RI_0_ RDFT_STORE_RI_0_
#endif

#ifndef RDFT_5_STORE_RI
#define RDFT_5_STORE_RI RDFT_STORE_RI
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
    u[0] = RDFT_5_LOAD(RDFT_5_INPUT_INDEX(1));
    v[0] = RDFT_5_LOAD(RDFT_5_INPUT_INDEX(4));
    RV_DFT2_CONJ(&u[0], &v[0]);
    u[1] = RDFT_5_LOAD(RDFT_5_INPUT_INDEX(2));
    v[1] = RDFT_5_LOAD(RDFT_5_INPUT_INDEX(3));
    RV_DFT2_CONJ(&u[1], &v[1]);
    {
      RV_VECTOR x0 = RDFT_5_LOAD(RDFT_5_INPUT_INDEX(0));
      RV_DFT2(&u[0], &u[1]);
      RV_VECTOR z0 = RV_ADD(x0, u[0]);
      RDFT_5_STORE_RI_0_(5, z0);
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
  RDFT_5_STORE_RI(RDFT_5_OUTPUT_INDEX(1), c[0], s[0]);
  RDFT_5_STORE_RI(RDFT_5_OUTPUT_INDEX(2), c[1], s[1]);
}

#undef RDFT_5_LOAD
#undef RDFT_5_STORE_RI_0_
#undef RDFT_5_STORE_RI
#undef RDFT_5_INPUT_INDEX
#undef RDFT_5_OUTPUT_INDEX
