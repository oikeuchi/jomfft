#include "BatchDft.h"

#include "bluestein/BatchDft.h"
#include "bluestein/EvenSizeBatchDft.h"
#include "cooleyturkey/BatchDft.h"
#include "cooleyturkey/EvenSizeBatchDft.h"
#include "core/EvenSizeStrideBatchDft.h"
#include "core/StrideBatchDft.h"
#include "singleton/EvenSizeStrideBatchDft.h"
#include "singleton/StrideBatchDft.h"
#include "square/EvenSizeBatchDft.h"

#define NS_(n) NS(RDft_##n)

static struct NS(RealToComplexFunction) *NS_(EvenSizeBatchDft_F_new)(
    struct NS(DataFactory) *data, Long dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    bool inplace, int num_threads) {
  Long n = dft_size;
  struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor(&n);
  if (n > 1) {
    if (dft_size < DFT_BLUESTEIN_MIN_SIZE) {
#if PRINT_MODEL
      printf("RDftSingleton_EvenSizeStrideBatchDft_F(%d,%ld;%d)\n",
             (int)dft_size, batch_size, num_threads);
#endif
      return NS(RDftSingleton_EvenSizeStrideBatchDft_F_new)(
          data, (int)dft_size, batch_size, x_stride_0, x_stride_1, y_stride_0,
          y_stride_1, num_threads);
    } else {
      return NS(RDftBluestein_EvenSizeBatchDft_F_new)(
          data, dft_size, batch_size, x_stride_0, x_stride_1, y_stride_0,
          y_stride_1, num_threads);
    }
  }
  if (dft_size <= (Long)INT_MAX &&
      NS(RDftCore_EvenSizeStrideBatchDft_HasKey)((int)dft_size)) {
#if PRINT_MODEL
    printf("RDftCore_EvenSizeStrideBatchDft_F(%d,%ld;%d)\n", (int)dft_size,
           batch_size, num_threads);
#endif
    return NS(RDftCore_EvenSizeStrideBatchDft_F_new)(
        data, (int)dft_size, batch_size, x_stride_0, x_stride_1, y_stride_0,
        y_stride_1, num_threads);
  }
  if (NS(RDftSquare_EvenSizeBatchDft_CanSplit)(&f, num_threads)) {
    return NS(RDftSquare_EvenSizeBatchDft_F_new)(
        data, &f, batch_size, x_stride_0, x_stride_1, y_stride_0, y_stride_1,
        inplace, num_threads);
  }
  return NS(RDftCooleyTurkey_EvenSizeBatchDft_F_new)(
      data, &f, batch_size, x_stride_0, x_stride_1, y_stride_0, y_stride_1,
      inplace, num_threads);
}

static struct NS(RealToComplexFunction) *NS_(OddSizeBatchDft_F_new)(
    struct NS(DataFactory) *data, Long dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
  Long n = dft_size;
  struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor(&n);
  if (n > 1) {
    if (dft_size < 2 * DFT_BLUESTEIN_MIN_SIZE) {
#if PRINT_MODEL
      printf("RDftSingleton_StrideBatchDft_F(%d,%ld;%d)\n", (int)dft_size,
             batch_size, num_threads);
#endif
      return NS(RDftSingleton_StrideBatchDft_F_new)(
          data, (int)dft_size, batch_size, x_stride_0, x_stride_1, y_stride_0,
          y_stride_1, num_threads);
    } else {
      return NS(RDftBluestein_BatchDft_F_new)(
          data, dft_size, batch_size, x_stride_0, x_stride_1, y_stride_0,
          y_stride_1, num_threads);
    }
  }
  return NS(RDftCooleyTurkey_BatchDft_F_new)(data, &f, batch_size, x_stride_0,
                                             x_stride_1, y_stride_0, y_stride_1,
                                             num_threads);
}

struct NS(RealToComplexFunction) *NS_(BatchDft_F_new)(
    struct NS(DataFactory) *data, Long dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    bool inplace, int num_threads) {
  if (dft_size <= (Long)INT_MAX &&
      NS(RDftCore_StrideBatchDft_HasKey)((int)dft_size)) {
#if PRINT_MODEL
    printf("RDftCore_StrideBatchDft_F(%d,%ld;%d)\n", (int)dft_size, batch_size,
           num_threads);
#endif
    return NS(RDftCore_StrideBatchDft_F_new)((int)dft_size, batch_size,
                                             x_stride_0, x_stride_1, y_stride_0,
                                             y_stride_1, num_threads);
  }
  if (dft_size % 2 == 0) {
    return NS_(EvenSizeBatchDft_F_new)(data, dft_size / 2, batch_size,
                                       x_stride_0, x_stride_1, y_stride_0,
                                       y_stride_1, inplace, num_threads);
  } else {
    return NS_(OddSizeBatchDft_F_new)(data, dft_size, batch_size, x_stride_0,
                                      x_stride_1, y_stride_0, y_stride_1,
                                      num_threads);
  }
}

static struct NS(ComplexToRealFunction) *NS_(EvenSizeBatchDft_B_new)(
    struct NS(DataFactory) *data, Long dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    bool inplace, int num_threads) {
  Long n = dft_size;
  struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor(&n);
  if (n > 1) {
    if (dft_size < DFT_BLUESTEIN_MIN_SIZE) {
#if PRINT_MODEL
      printf("RDftSingleton_EvenSizeStrideBatchDft_B(%d,%ld;%d)\n",
             (int)dft_size, batch_size, num_threads);
#endif
      return NS(RDftSingleton_EvenSizeStrideBatchDft_B_new)(
          data, (int)dft_size, batch_size, x_stride_0, x_stride_1, y_stride_0,
          y_stride_1, num_threads);
    } else {
      return NS(RDftBluestein_EvenSizeBatchDft_B_new)(
          data, dft_size, batch_size, x_stride_0, x_stride_1, y_stride_0,
          y_stride_1, num_threads);
    }
  }
  if (dft_size <= (Long)INT_MAX &&
      NS(RDftCore_EvenSizeStrideBatchDft_HasKey)((int)dft_size)) {
#if PRINT_MODEL
    printf("RDftCore_EvenSizeStrideBatchDft_B(%d,%ld;%d)\n", (int)dft_size,
           batch_size, num_threads);
#endif
    return NS(RDftCore_EvenSizeStrideBatchDft_B_new)(
        data, (int)dft_size, batch_size, x_stride_0, x_stride_1, y_stride_0,
        y_stride_1, num_threads);
  }
  if (NS(RDftSquare_EvenSizeBatchDft_CanSplit)(&f, num_threads)) {
    return NS(RDftSquare_EvenSizeBatchDft_B_new)(
        data, &f, batch_size, x_stride_0, x_stride_1, y_stride_0, y_stride_1,
        inplace, num_threads);
  }
  return NS(RDftCooleyTurkey_EvenSizeBatchDft_B_new)(
      data, &f, batch_size, x_stride_0, x_stride_1, y_stride_0, y_stride_1,
      inplace, num_threads);
}

static struct NS(ComplexToRealFunction) *NS_(OddSizeBatchDft_B_new)(
    struct NS(DataFactory) *data, Long dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
  Long n = dft_size;
  struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor(&n);
  if (n > 1) {
    if (dft_size < 2 * DFT_BLUESTEIN_MIN_SIZE) {
#if PRINT_MODEL
      printf("RDftSingleton_StrideBatchDft_B(%d,%ld;%d)\n", (int)dft_size,
             batch_size, num_threads);
#endif
      return NS(RDftSingleton_StrideBatchDft_B_new)(
          data, (int)dft_size, batch_size, x_stride_0, x_stride_1, y_stride_0,
          y_stride_1, num_threads);
    } else {
      return NS(RDftBluestein_BatchDft_B_new)(
          data, dft_size, batch_size, x_stride_0, x_stride_1, y_stride_0,
          y_stride_1, num_threads);
    }
  }
  return NS(RDftCooleyTurkey_BatchDft_B_new)(data, &f, batch_size, x_stride_0,
                                             x_stride_1, y_stride_0, y_stride_1,
                                             num_threads);
}

struct NS(ComplexToRealFunction) *NS_(BatchDft_B_new)(
    struct NS(DataFactory) *data, Long dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    bool inplace, int num_threads) {
  if (dft_size <= (Long)INT_MAX &&
      NS(RDftCore_StrideBatchDft_HasKey)((int)dft_size)) {
#if PRINT_MODEL
    printf("RDftCore_StrideBatchDft_B(%d,%ld;%d)\n", (int)dft_size, batch_size,
           num_threads);
#endif
    return NS(RDftCore_StrideBatchDft_B_new)((int)dft_size, batch_size,
                                             x_stride_0, x_stride_1, y_stride_0,
                                             y_stride_1, num_threads);
  }
  if (dft_size % 2 == 0) {
    return NS_(EvenSizeBatchDft_B_new)(data, dft_size / 2, batch_size,
                                       x_stride_0, x_stride_1, y_stride_0,
                                       y_stride_1, inplace, num_threads);
  } else {
    return NS_(OddSizeBatchDft_B_new)(data, dft_size, batch_size, x_stride_0,
                                      x_stride_1, y_stride_0, y_stride_1,
                                      num_threads);
  }
}
