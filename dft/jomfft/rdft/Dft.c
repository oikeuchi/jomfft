#include "Dft.h"

#include "../cdft/bluestein/Dft.h"
#include "bluestein/Dft.h"
#include "bluestein/EvenSizeDft.h"
#include "cooleyturkey/Dft.h"
#include "cooleyturkey/EvenSizeDft.h"
#include "core/EvenSizeStrideBatchDft.h"
#include "core/StrideBatchDft.h"
#include "singleton/EvenSizeStrideBatchDft.h"
#include "singleton/StrideBatchDft.h"
#include "square/EvenSizeDft.h"

#define EVEN_SIZE_STRIDEDBATCHDFT_MAX_SIZE 64

#define NS_(n) NS(RDft_##n)

static struct NS(RealToComplexFunction) *NS_(EvenSizeDft_F_new)(
    struct NS(DataFactory) *data, Long dft_size, Long x_stride, Long y_stride,
    bool inplace, int num_threads) {
  Long n = dft_size;
  struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor(&n);
  if (n > 1) {
    if (dft_size < DFT_BLUESTEIN_MIN_SIZE) {
      return NS(RDftSingleton_EvenSizeStrideBatchDft_F_new)(
          data, (int)dft_size, 1, x_stride, 1, y_stride, 1, 1);
    } else {
      if (NS(CDftBluestein_ConvolutionSize)(dft_size) <= C_L1_CACHE_SIZE) {
        num_threads = 1;
      }
      return NS(RDftBluestein_EvenSizeDft_F_new)(data, dft_size, x_stride,
                                                 y_stride, num_threads);
    }
  }
  if (dft_size <= EVEN_SIZE_STRIDEDBATCHDFT_MAX_SIZE &&
      NS(RDftCore_EvenSizeStrideBatchDft_HasKey)((int)dft_size)) {
    return NS(RDftCore_EvenSizeStrideBatchDft_F_new)(
        data, (int)dft_size, 1, x_stride, 1, y_stride, 1, 1);
  }
  if (dft_size <= C_L1_CACHE_SIZE) {
    num_threads = 1;
  }
  if (NS(RDftSquare_EvenSizeDft_CanSplit)(&f, num_threads)) {
    return NS(RDftSquare_EvenSizeDft_F_new)(data, &f, x_stride, y_stride,
                                            inplace, num_threads);
  }
  return NS(RDftCooleyTurkey_EvenSizeDft_F_new)(data, &f, x_stride, y_stride,
                                                inplace, num_threads);
}

static struct NS(RealToComplexFunction) *NS_(OddSizeDft_F_new)(
    struct NS(DataFactory) *data, Long dft_size, Long x_stride, Long y_stride,
    int num_threads) {
  Long n = dft_size;
  struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor(&n);
  if (n > 1) {
    if (dft_size < 2 * DFT_BLUESTEIN_MIN_SIZE) {
#if PRINT_MODEL
      printf("RDftSingleton_StrideBatchDft_F(%d,1;%d)\n", (int)dft_size,
             num_threads);
#endif
      return NS(RDftSingleton_StrideBatchDft_F_new)(
          data, (int)dft_size, 1, x_stride, 1, y_stride, 1, 1);
    } else {
      if (NS(CDftBluestein_ConvolutionSize)(dft_size) <= C_L1_CACHE_SIZE) {
        num_threads = 1;
      }
      return NS(RDftBluestein_Dft_F_new)(data, dft_size, x_stride, y_stride,
                                         num_threads);
    }
  }
  if (dft_size <= C_L1_CACHE_SIZE) {
    num_threads = 1;
  }
  return NS(RDftCooleyTurkey_Dft_F_new)(data, &f, x_stride, y_stride,
                                        num_threads);
}

struct NS(RealToComplexFunction) *NS_(Dft_F_new)(struct NS(DataFactory) *data,
                                                 Long dft_size, Long x_stride,
                                                 Long y_stride, bool inplace,
                                                 int num_threads) {
  if (dft_size <= (Long)INT_MAX &&
      NS(RDftCore_StrideBatchDft_HasKey)((int)dft_size)) {
#if PRINT_MODEL
    printf("RDftCore_StrideBatchDft_F(%d,1;%d)\n", (int)dft_size, num_threads);
#endif
    return NS(RDftCore_StrideBatchDft_F_new)((int)dft_size, 1, x_stride, 1,
                                             y_stride, 1, 1);
  }
  if (dft_size % 2 == 0) {
    return NS_(EvenSizeDft_F_new)(data, dft_size / 2, x_stride, y_stride,
                                  inplace, num_threads);
  } else {
    return NS_(OddSizeDft_F_new)(data, dft_size, x_stride, y_stride,
                                 num_threads);
  }
}

static struct NS(ComplexToRealFunction) *NS_(EvenSizeDft_B_new)(
    struct NS(DataFactory) *data, Long dft_size, Long x_stride, Long y_stride,
    bool inplace, int num_threads) {
  Long n = dft_size;
  struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor(&n);
  if (n > 1) {
    if (dft_size < DFT_BLUESTEIN_MIN_SIZE) {
      return NS(RDftSingleton_EvenSizeStrideBatchDft_B_new)(
          data, (int)dft_size, 1, x_stride, 1, y_stride, 1, 1);
    } else {
      if (NS(CDftBluestein_ConvolutionSize)(dft_size) <= C_L1_CACHE_SIZE) {
        num_threads = 1;
      }
      return NS(RDftBluestein_EvenSizeDft_B_new)(data, dft_size, x_stride,
                                                 y_stride, num_threads);
    }
  }
  if (dft_size <= EVEN_SIZE_STRIDEDBATCHDFT_MAX_SIZE &&
      NS(RDftCore_EvenSizeStrideBatchDft_HasKey)((int)dft_size)) {
    return NS(RDftCore_EvenSizeStrideBatchDft_B_new)(
        data, (int)dft_size, 1, x_stride, 1, y_stride, 1, 1);
  }
  if (dft_size <= C_L1_CACHE_SIZE) {
    num_threads = 1;
  }
  if (NS(RDftSquare_EvenSizeDft_CanSplit)(&f, num_threads)) {
    return NS(RDftSquare_EvenSizeDft_B_new)(data, &f, x_stride, y_stride,
                                            inplace, num_threads);
  }
  return NS(RDftCooleyTurkey_EvenSizeDft_B_new)(data, &f, x_stride, y_stride,
                                                inplace, num_threads);
}

static struct NS(ComplexToRealFunction) *NS_(OddSizeDft_B_new)(
    struct NS(DataFactory) *data, Long dft_size, Long x_stride, Long y_stride,
    int num_threads) {
  Long n = dft_size;
  struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor(&n);
  if (n > 1) {
    if (dft_size < 2 * DFT_BLUESTEIN_MIN_SIZE) {
#if PRINT_MODEL
      printf("RDftSingleton_StrideBatchDft_B(%d,1;%d)\n", (int)dft_size,
             num_threads);
#endif
      return NS(RDftSingleton_StrideBatchDft_B_new)(
          data, (int)dft_size, 1, x_stride, 1, y_stride, 1, 1);
    } else {
      if (NS(CDftBluestein_ConvolutionSize)(dft_size) <= C_L1_CACHE_SIZE) {
        num_threads = 1;
      }
      return NS(RDftBluestein_Dft_B_new)(data, dft_size, x_stride, y_stride,
                                         num_threads);
    }
  }
  if (dft_size <= C_L1_CACHE_SIZE) {
    num_threads = 1;
  }
  return NS(RDftCooleyTurkey_Dft_B_new)(data, &f, x_stride, y_stride,
                                        num_threads);
}

struct NS(ComplexToRealFunction) *NS_(Dft_B_new)(struct NS(DataFactory) *data,
                                                 Long dft_size, Long x_stride,
                                                 Long y_stride, bool inplace,
                                                 int num_threads) {
  if (dft_size <= (Long)INT_MAX &&
      NS(RDftCore_StrideBatchDft_HasKey)((int)dft_size)) {
#if PRINT_MODEL
    printf("RDftCore_StrideBatchDft_B(%d,1;%d)\n", (int)dft_size, num_threads);
#endif
    return NS(RDftCore_StrideBatchDft_B_new)((int)dft_size, 1, x_stride, 1,
                                             y_stride, 1, 1);
  }
  if (dft_size % 2 == 0) {
    return NS_(EvenSizeDft_B_new)(data, dft_size / 2, x_stride, y_stride,
                                  inplace, num_threads);
  } else {
    return NS_(OddSizeDft_B_new)(data, dft_size, x_stride, y_stride,
                                 num_threads);
  }
}
