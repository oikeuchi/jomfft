#include <jomfft/math/constants/SinCos_9.h>

#ifndef CDFT_9_LOAD
#define CDFT_9_LOAD CDFT_LOAD
#endif

#ifndef CDFT_9_STORE
#define CDFT_9_STORE CDFT_STORE
#endif

#ifndef CDFT_9_INPUT_INDEX
#define CDFT_9_INPUT_INDEX(i) (i)
#endif

#ifndef CDFT_9_OUTPUT_INDEX
#define CDFT_9_OUTPUT_INDEX(i) (i)
#endif

{
  CV_VECTOR z_9__[9];
#define CDFT_3_LOAD CDFT_9_LOAD
#define CDFT_3_INPUT_INDEX(i) CDFT_9_INPUT_INDEX(3 * (i))
#define CDFT_3_STORE(i, x) z_9__[(i)] = x
#include "Dft_3.h"
#define CDFT_3_LOAD CDFT_9_LOAD
#define CDFT_3_INPUT_INDEX(i) CDFT_9_INPUT_INDEX(3 * (i) + 1)
#define CDFT_3_STORE(i, x) z_9__[(i) + 3] = x
#include "Dft_3.h"
#define CDFT_3_LOAD CDFT_9_LOAD
#define CDFT_3_INPUT_INDEX(i) CDFT_9_INPUT_INDEX(3 * (i) + 2)
#define CDFT_3_STORE(i, x) z_9__[(i) + 6] = x
#include "Dft_3.h"
  z_9__[4] = CV_C_MUL_RI(z_9__[4], RV_SCALAR(COS2PI_9), RV_SCALAR(SIN2PI_9));
  z_9__[5] = CV_C_MUL_RI(z_9__[5], RV_SCALAR(COS4PI_9), RV_SCALAR(SIN4PI_9));
  z_9__[7] = CV_C_MUL_RI(z_9__[7], RV_SCALAR(COS4PI_9), RV_SCALAR(SIN4PI_9));
  z_9__[8] = CV_C_MUL_RI(z_9__[8], RV_SCALAR(COS8PI_9), RV_SCALAR(SIN8PI_9));
#define CDFT_3_LOAD(i, z) *(z) = z_9__[3 * (i)]
#define CDFT_3_STORE CDFT_9_STORE
#define CDFT_3_OUTPUT_INDEX(i) CDFT_9_OUTPUT_INDEX(3 * (i))
#include "Dft_3.h"
#define CDFT_3_LOAD(i, z) *(z) = z_9__[3 * (i) + 1]
#define CDFT_3_STORE CDFT_9_STORE
#define CDFT_3_OUTPUT_INDEX(i) CDFT_9_OUTPUT_INDEX(3 * (i) + 1)
#include "Dft_3.h"
#define CDFT_3_LOAD(i, z) *(z) = z_9__[3 * (i) + 2]
#define CDFT_3_STORE CDFT_9_STORE
#define CDFT_3_OUTPUT_INDEX(i) CDFT_9_OUTPUT_INDEX(3 * (i) + 2)
#include "Dft_3.h"
}

#undef CDFT_9_LOAD
#undef CDFT_9_STORE
#undef CDFT_9_INPUT_INDEX
#undef CDFT_9_OUTPUT_INDEX
