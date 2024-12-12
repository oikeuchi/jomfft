#ifndef CDFT_12_LOAD
#define CDFT_12_LOAD CDFT_LOAD
#endif

#ifndef CDFT_12_STORE
#define CDFT_12_STORE CDFT_STORE
#endif

#ifndef CDFT_12_INPUT_INDEX
#define CDFT_12_INPUT_INDEX(i) (i)
#endif

#ifndef CDFT_12_OUTPUT_INDEX
#define CDFT_12_OUTPUT_INDEX(i) (i)
#endif

{
  const int i_indexes_12__[] = {0, 9, 6, 3, 4, 1, 10, 7, 8, 5, 2, 11};
  const int o_indexes_12__[] = {0, 3, 6, 9, 4, 7, 10, 1, 8, 11, 2, 5};
  CV_VECTOR z_12__[12];
#define CDFT_4_LOAD CDFT_12_LOAD
#define CDFT_4_INPUT_INDEX(i) (i_indexes_12__[(i)])
#define CDFT_4_STORE(i, x) z_12__[(i)] = x
#include "Dft_4.h"
#define CDFT_4_LOAD CDFT_12_LOAD
#define CDFT_4_INPUT_INDEX(i) (i_indexes_12__[(i) + 4])
#define CDFT_4_STORE(i, x) z_12__[(i) + 4] = x
#include "Dft_4.h"
#define CDFT_4_LOAD CDFT_12_LOAD
#define CDFT_4_INPUT_INDEX(i) (i_indexes_12__[(i) + 8])
#define CDFT_4_STORE(i, x) z_12__[(i) + 8] = x
#include "Dft_4.h"
#define CDFT_3_LOAD(i, z) *(z) = z_12__[4 * (i)]
#define CDFT_3_OUTPUT_INDEX(i) o_indexes_12__[4 * (i)]
#include "Dft_3.h"
#define CDFT_3_LOAD(i, z) *(z) = z_12__[4 * (i) + 1]
#define CDFT_3_STORE CDFT_12_STORE
#define CDFT_3_OUTPUT_INDEX(i) o_indexes_12__[4 * (i) + 1]
#include "Dft_3.h"
#define CDFT_3_LOAD(i, z) *(z) = z_12__[4 * (i) + 2]
#define CDFT_3_STORE CDFT_12_STORE
#define CDFT_3_OUTPUT_INDEX(i) o_indexes_12__[4 * (i) + 2]
#include "Dft_3.h"
#define CDFT_3_LOAD(i, z) *(z) = z_12__[4 * (i) + 3]
#define CDFT_3_STORE CDFT_12_STORE
#define CDFT_3_OUTPUT_INDEX(i) o_indexes_12__[4 * (i) + 3]
#include "Dft_3.h"
}

#undef CDFT_12_LOAD
#undef CDFT_12_STORE
#undef CDFT_12_INPUT_INDEX
#undef CDFT_12_OUTPUT_INDEX
