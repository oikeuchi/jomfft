#ifndef CDFT_6_LOAD
#define CDFT_6_LOAD CDFT_LOAD
#endif

#ifndef CDFT_6_STORE
#define CDFT_6_STORE CDFT_STORE
#endif

{
  const int i_indexes_6__[] = {0, 3, 4, 1, 2, 5};
  const int o_indexes_6__[] = {0, 3, 2, 5, 4, 1};
  CV_VECTOR z_6__[6];
#define CDFT_2_LOAD CDFT_6_LOAD
#define CDFT_2_INPUT_INDEX(i) (i_indexes_6__[(i)])
#define CDFT_2_STORE(i, x) z_6__[(i)] = x
#include "Dft_2.h"
#define CDFT_2_LOAD CDFT_6_LOAD
#define CDFT_2_INPUT_INDEX(i) (i_indexes_6__[(i) + 2])
#define CDFT_2_STORE(i, x) z_6__[(i) + 2] = x
#include "Dft_2.h"
#define CDFT_2_LOAD CDFT_6_LOAD
#define CDFT_2_INPUT_INDEX(i) (i_indexes_6__[(i) + 4])
#define CDFT_2_STORE(i, x) z_6__[(i) + 4] = x
#include "Dft_2.h"
#define CDFT_3_LOAD(i, z) *(z) = z_6__[2 * (i)]
#define CDFT_3_STORE CDFT_6_STORE
#define CDFT_3_OUTPUT_INDEX(i) o_indexes_6__[2 * (i)]
#include "Dft_3.h"
#define CDFT_3_LOAD(i, z) *(z) = z_6__[2 * (i) + 1]
#define CDFT_3_STORE CDFT_6_STORE
#define CDFT_3_OUTPUT_INDEX(i) o_indexes_6__[2 * (i) + 1]
#include "Dft_3.h"
}

#undef CDFT_6_LOAD
#undef CDFT_6_STORE
