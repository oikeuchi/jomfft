#ifndef CDFT_10_LOAD
#define CDFT_10_LOAD CDFT_LOAD
#endif

#ifndef CDFT_10_STORE
#define CDFT_10_STORE CDFT_STORE
#endif

#ifndef CDFT_10_INPUT_INDEX
#define CDFT_10_INPUT_INDEX(i) (i)
#endif

#ifndef CDFT_10_OUTPUT_INDEX
#define CDFT_10_OUTPUT_INDEX(i) (i)
#endif

{
  const int i_indexes_10__[] = {0, 5, 6, 1, 2, 7, 8, 3, 4, 9};
  const int o_indexes_10__[] = {0, 5, 2, 7, 4, 9, 6, 1, 8, 3};
  CV_VECTOR z_10__[10];
#define CDFT_2_LOAD CDFT_10_LOAD
#define CDFT_2_INPUT_INDEX(i) (i_indexes_10__[(i)])
#define CDFT_2_STORE(i, x) z_10__[(i)] = x
#include "Dft_2.h"
#define CDFT_2_LOAD CDFT_10_LOAD
#define CDFT_2_INPUT_INDEX(i) (i_indexes_10__[(i) + 2])
#define CDFT_2_STORE(i, x) z_10__[(i) + 2] = x
#include "Dft_2.h"
#define CDFT_2_LOAD CDFT_10_LOAD
#define CDFT_2_INPUT_INDEX(i) (i_indexes_10__[(i) + 4])
#define CDFT_2_STORE(i, x) z_10__[(i) + 4] = x
#include "Dft_2.h"
#define CDFT_2_LOAD CDFT_10_LOAD
#define CDFT_2_INPUT_INDEX(i) (i_indexes_10__[(i) + 6])
#define CDFT_2_STORE(i, x) z_10__[(i) + 6] = x
#include "Dft_2.h"
#define CDFT_2_LOAD CDFT_10_LOAD
#define CDFT_2_INPUT_INDEX(i) (i_indexes_10__[(i) + 8])
#define CDFT_2_STORE(i, x) z_10__[(i) + 8] = x
#include "Dft_2.h"
#define CDFT_5_LOAD(i, z) *(z) = z_10__[2 * (i)]
#define CDFT_5_STORE CDFT_10_STORE
#define CDFT_5_OUTPUT_INDEX(i) o_indexes_10__[2 * (i)]
#include "Dft_5.h"
#define CDFT_5_LOAD(i, z) *(z) = z_10__[2 * (i) + 1]
#define CDFT_5_STORE CDFT_10_STORE
#define CDFT_5_OUTPUT_INDEX(i) o_indexes_10__[2 * (i) + 1]
#include "Dft_5.h"
}

#undef CDFT_10_LOAD
#undef CDFT_10_STORE
#undef CDFT_10_INPUT_INDEX
#undef CDFT_10_OUTPUT_INDEX
