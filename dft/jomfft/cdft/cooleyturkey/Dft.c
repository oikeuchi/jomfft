#include "Dft.h"

#include <jomfft/util/IntUtil.h>
#include <jomfft/util/SplitUtil.h>

#include "../../model/ComplexCopy.h"
#include "../../model/ComplexFunctionBuffered.h"
#include "../../model/ComplexFunctionLoop.h"
#include "../../model/ComplexFunctionPair.h"
#include "../../model/ComplexFunctionPairW.h"
#include "../../model/ThreadingUtil.h"
#include "../core/StrideBatchDft.h"
#include "../core/TwiddleDft.h"
#include "TransposeDft.h"

#define PAIR_MAX_SIZE (C_L1_CACHE_SIZE / 4)
#define MAX_LAST_SIZE 8

#define NS_(n) NS(CDftCooleyTurkey_##n)

static struct jomfftIntVector *NS_(Dft_Split_0)(
    const struct jomfftPrimeFactors *dft_factors) {
  struct jomfftIntVector *dft_keys = NS(CDftCore_StrideBatchDft_Keys)();
  struct jomfftIntVector *twiddle_keys = NS(CDftCore_TwiddleDft_Keys)();
  struct jomfftPrimeFactors primes = *dft_factors;
  int first = 0;
  int second = 0;
  jomfftGetDftSizePair(&primes, dft_keys, twiddle_keys, PAIR_MAX_SIZE, &first,
                       &second);
  struct jomfftIntVector *out = NULL;
  if (jomfftPrimeFactors_empty(&primes)) {
    out = jomfftIntVector_new();
    jomfftIntVector_append(out, first);
    jomfftIntVector_append(out, second);
  }
  jomfftIntVector_delete(dft_keys);
  jomfftIntVector_delete(twiddle_keys);
  return out;
}

static struct jomfftIntVector *NS_(Dft_Split_1)(
    const struct jomfftPrimeFactors *dft_factors, int num_threads) {
  struct jomfftIntVector *dft_keys = NS(CDftCore_StrideBatchDft_Keys)();
  struct jomfftIntVector *twiddle_keys = NS(CDftCore_TwiddleDft_Keys)();
  struct jomfftPrimeFactors primes = *dft_factors;
  struct jomfftIntVector *out = jomfftIntVector_new();
  int last = 0;
  if (num_threads > 1) {
    if (jomfftPrimeFactors_hasFactor(&primes, num_threads) &&
        jomfftIntVector_binarySearch(twiddle_keys, num_threads) >= 0) {
      last = jomfftGetDftSizeWithFactor(&primes, twiddle_keys, num_threads,
                                        MAX(num_threads, MAX_LAST_SIZE));
    } else {
      last = jomfftGetDftSizeNearestToMultiple(
          &primes, twiddle_keys, num_threads,
          MAX(4 * num_threads, MAX_LAST_SIZE));
    }
  } else {
    last = jomfftGetDftSize_(&primes, twiddle_keys, MAX_LAST_SIZE);
  }
  assert(last > 1);
  struct jomfftPrimeFactors last_f = jomfftPrimeFactors_Factor_(last);
  if (jomfftPrimeFactors_empty(&primes)) {
    int i = 0;
    while (i < NUM_OF_PRIMES && last_f.exponents[i] == 0) {
      ++i;
    }
    assert(i < NUM_OF_PRIMES && last_f.exponents[i] > 0);
    last_f.exponents[i] -= 1;
    jomfftIntVector_append(out, jomfftPrimeFactors_Prime(i));
    jomfftIntVector_append(out, jomfftPrimeFactors_product(&last_f));
  } else {
    if (jomfftPrimeFactors_count(&primes) == 1) {
      jomfftIntVector_append(out, jomfftPrimeFactors_product(&primes));
    } else {
      int first = 0;
      int second = 0;
      jomfftGetDftSizePair(&primes, dft_keys, twiddle_keys, PAIR_MAX_SIZE,
                           &first, &second);
      jomfftIntVector_append(out, first);
      jomfftIntVector_append(out, second);
      while (!jomfftPrimeFactors_empty(&primes)) {
        jomfftIntVector_append(out, jomfftGetDftSize(&primes, twiddle_keys));
      }
    }
    jomfftIntVector_append(out, last);
  }
  jomfftIntVector_delete(dft_keys);
  jomfftIntVector_delete(twiddle_keys);
  return out;
}

struct jomfftIntVector *NS_(Dft_Split)(
    const struct jomfftPrimeFactors *dft_factors, int num_threads) {
  struct jomfftIntVector *out = NS_(Dft_Split_0)(dft_factors);
  if (!out) {
    out = NS_(Dft_Split_1)(dft_factors, num_threads);
  }
  assert(jomfftIntVector_size(out) >= 2);
  return out;
}

static struct NS(ComplexFunction) *NS_(Dft_create)(
    struct NS(DataFactory) *data, bool backward, Long i, const int *dft_sizes,
    Long x_stride, Long internal_y_stride, int num_threads) {
  assert(i >= 1);
  if (i == 1) {
    const Long n = dft_sizes[i];
    const Long s = internal_y_stride / n;
    return NS(ComplexFunctionPair_1_new)(
        NS(CDftCore_StrideBatchDft_new_)(backward, dft_sizes[0], n,
                                         x_stride * n, x_stride, 1,
                                         dft_sizes[0], num_threads),
        NS(CDftCore_TwiddleDft_new_)(data, backward, n, s, num_threads));
  } else {
    const Long n = dft_sizes[i];
    const Long s = internal_y_stride / n;
    int num_threads_0 = 1;
    int num_threads_1 = 1;
    NS(Threading_Split_)(num_threads, n, 1, s, &num_threads_0, &num_threads_1,
                         true);
    return NS(ComplexFunctionPair_1_new)(
        NS(ComplexFunctionLoop_new_)(NS_(Dft_create)(data, backward, i - 1,
                                                     dft_sizes, x_stride * n, s,
                                                     num_threads_1),
                                     n, x_stride, s, num_threads_0),
        NS(CDftCore_TwiddleDft_new_)(data, backward, n, s, num_threads));
  }
}

struct NS(ComplexFunction) *NS_(Dft_new_1)(struct NS(DataFactory) *data,
                                           bool backward, Long dimension,
                                           const int *dft_sizes, Long x_stride,
                                           int num_threads) {
  assert(dimension >= 2);
#if PRINT_MODEL
  printf("CooleyTurkey_Dft(%s", BOOL_STR(backward));
  for (Long i = 0; i < dimension; ++i) {
    printf(",%d", dft_sizes[i]);
  }
  printf(";%d)\n", num_threads);
#endif
  const Long m = dimension;
  const Long dft_size = jomfftInt_ProductLong(m, dft_sizes);
  return NS_(Dft_create)(data, backward, m - 1, dft_sizes, x_stride, dft_size,
                         num_threads);
}

static struct NS(ComplexFunctionW) *NS_(Dft_newW_)(
    struct NS(DataFactory) *data, bool backward, Long dimension,
    const int *dft_sizes, Long x_stride, Long y_stride, int num_threads) {
  const Long dft_size = jomfftInt_ProductLong(dimension, dft_sizes);
  return NS(ComplexFunctionPairW_new)(
      NS_(Dft_new_1)(data, backward, dimension, dft_sizes, x_stride,
                     num_threads),
      NS(Complex_Copy_new_)(dft_size, 1, y_stride, num_threads),
      dft_size * sizeof(Complex));
}

struct NS(ComplexFunction) *NS_(Dft_new)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long x_stride, Long y_stride,
    bool inplace, int num_threads) {
  struct jomfftIntVector *dft_sizes = NS_(Dft_Split)(dft_factors, num_threads);
  const Long dimension = jomfftIntVector_size(dft_sizes);
  const int *dft_sizes_ = jomfftIntVector_data(dft_sizes);
  struct NS(ComplexFunction) *out = NULL;
  if (inplace || y_stride != 1) {
    out = NS(ComplexFunctionBuffered_new)(
        NS_(Dft_newW_)(data, backward, dimension, dft_sizes_, x_stride,
                       y_stride, num_threads));
  } else {
    out = NS_(Dft_new_1)(data, backward, dimension, dft_sizes_, x_stride,
                         num_threads);
  }
  jomfftIntVector_delete(dft_sizes);
  return out;
}

struct NS(ComplexFunctionW) *NS_(Dft_newW)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long x_stride, Long y_stride,
    int num_threads) {
  struct jomfftIntVector *dft_sizes = NS_(Dft_Split)(dft_factors, num_threads);
  struct NS(ComplexFunctionW) *out = NS_(Dft_newW_)(
      data, backward, jomfftIntVector_size(dft_sizes),
      jomfftIntVector_data(dft_sizes), x_stride, y_stride, num_threads);
  jomfftIntVector_delete(dft_sizes);
  return out;
}
