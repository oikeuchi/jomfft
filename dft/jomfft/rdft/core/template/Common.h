#include <jomfft/math/constants/Constants.h>

#ifdef CV_SIZE
#include "../../../simd/Complex_V.h"
#endif
#ifdef RV_SIZE
#include "../../../simd/Real_V.h"
#include "../../../simd/SplitComplex.h"

#define RDFT_LOAD_ARRAY_RI_0(arr, x0, xh)                                      \
  do {                                                                         \
    *(x0) = (arr)[0];                                                          \
    *(xh) = (arr)[1];                                                          \
  } while (0)

#define RDFT_LOAD_ARRAY_RI(arr, i, x_r, x_i)                                   \
  do {                                                                         \
    *(x_r) = (arr)[2 * (i)];                                                   \
    *(x_i) = (arr)[2 * (i) + 1];                                               \
  } while (0)

#define RDFT_STORE_ARRAY_RI_0(arr, x0, xh)                                     \
  do {                                                                         \
    (arr)[0] = (x0);                                                           \
    (arr)[1] = (xh);                                                           \
  } while (0)

#define RDFT_STORE_ARRAY_RI(arr, i, x_r, x_i)                                  \
  do {                                                                         \
    (arr)[2 * (i)] = (x_r);                                                    \
    (arr)[2 * (i) + 1] = (x_i);                                                \
  } while (0)
#endif
