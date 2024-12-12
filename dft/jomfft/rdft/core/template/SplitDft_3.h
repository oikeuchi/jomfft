#include <jomfft/math/constants/SinCos_3.h>

#ifndef SDFT_3_LOAD
#define SDFT_3_LOAD SDFT_LOAD
#endif

#ifndef SDFT_3_STORE
#define SDFT_3_STORE SDFT_STORE
#endif

#ifndef SDFT_3_INPUT_INDEX
#define SDFT_3_INPUT_INDEX(i) (i)
#endif

#ifndef SDFT_3_OUTPUT_INDEX
#define SDFT_3_OUTPUT_INDEX(i) (i)
#endif

{
  RV_VECTOR x0[2];
  RV_VECTOR u[2];
  RV_VECTOR v[2];
  SDFT_3_LOAD(SDFT_3_INPUT_INDEX(0), x0);
  SDFT_3_LOAD(SDFT_3_INPUT_INDEX(1), u);
  SDFT_3_LOAD(SDFT_3_INPUT_INDEX(2), v);
  SV_DFT2(u, v);
  {
    RV_VECTOR z0[2];
    SV_ADD(x0, u, z0);
    SDFT_3_STORE(SDFT_3_OUTPUT_INDEX(0), z0);
  }
  SV_FMADD_(u, RV_SCALAR(COS2PI_3), x0);
  SV_MUL_R_(v, RV_SCALAR(SIN2PI_3));
  SV_C_MULI_DFT2(u, v);
  SDFT_3_STORE(SDFT_3_OUTPUT_INDEX(1), u);
  SDFT_3_STORE(SDFT_3_OUTPUT_INDEX(2), v);
}

#undef SDFT_3_LOAD
#undef SDFT_3_STORE
#undef SDFT_3_INPUT_INDEX
#undef SDFT_3_OUTPUT_INDEX
