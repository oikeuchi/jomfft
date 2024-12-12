#include "TransposeDft.h"

#include "../../model/ComplexFunctionLoop.h"
#include "../../model/ComplexFunctionPair.h"
#include "../core/StrideBatchDft.h"
#include "../core/TwiddleDft.h"
#include <jomfft/util/IntUtil.h>
#include <jomfft/util/SplitUtil.h>

#define PAIR_MAX_SIZE (C_L1_CACHE_SIZE / 4)

#define NS_(n) NS(CDftCooleyTurkey_##n)

static struct jomfftIntVector *NS_(TransposeDft_Split_0)(
    const struct jomfftPrimeFactors *dft_factors) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  if (dft_size <= PAIR_MAX_SIZE) {
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
  } else {
    return NULL;
  }
}

static struct jomfftIntVector *NS_(TransposeDft_Split_1)(
    const struct jomfftPrimeFactors *dft_factors) {
  struct jomfftIntVector *dft_keys = NS(CDftCore_StrideBatchDft_Keys)();
  struct jomfftIntVector *twiddle_keys = NS(CDftCore_TwiddleDft_Keys)();
  struct jomfftIntVector *out = jomfftIntVector_new();
  struct jomfftPrimeFactors primes = *dft_factors;
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  int first = 0;
  int second = 0;
  if (dft_size > 4 && primes.exponents[0] >= 2) {
    if (dft_size > 8 && primes.exponents[0] >= 3 &&
        (C_MEMORY_BUS_SIZE >= 8 ||
         C_MEMORY_BUS_SIZE * dft_size <= C_L2_CACHE_SIZE)) {
      primes.exponents[0] -= 3;
      first = 8;
    } else {
      primes.exponents[0] -= 2;
      first = 4;
    }
  } else {
#if C_SIMD_SIZE_S >= 4
    const int priorities[] = {3, 1, 2, 0};
#else
    const int priorities[] = {3, 2, 1, 0};
#endif
    int i = 0;
    while (i < NUM_OF_PRIMES && primes.exponents[priorities[i]] == 0) {
      ++i;
    }
    assert(i < NUM_OF_PRIMES);
    const int p = priorities[i];
    primes.exponents[p] -= 1;
    first = jomfftPrimeFactors_Prime(p);
  }
  second = jomfftGetDftSize_(&primes, twiddle_keys, PAIR_MAX_SIZE / first);
  jomfftIntVector_append(out, first);
  jomfftIntVector_append(out, second);
  while (!jomfftPrimeFactors_empty(&primes)) {
    jomfftIntVector_append(out, jomfftGetDftSize(&primes, twiddle_keys));
  }
  jomfftIntVector_delete(dft_keys);
  jomfftIntVector_delete(twiddle_keys);
  return out;
}

struct jomfftIntVector *NS_(TransposeDft_Split)(
    const struct jomfftPrimeFactors *dft_factors) {
  struct jomfftIntVector *out = NS_(TransposeDft_Split_0)(dft_factors);
  if (!out) {
    out = NS_(TransposeDft_Split_1)(dft_factors);
  }
  assert(jomfftIntVector_size(out) >= 2);
  return out;
}

static struct NS(ComplexFunction) *NS_(TransposeDft_create)(
    struct NS(DataFactory) *data, bool backward, Long i, const int *dft_sizes,
    Long batch_size, Long x_stride_0, Long x_stride_1, Long y_stride,
    Long internal_y_stride) {
  if (i == 0) {
    return NS(CDftCore_StrideBatchDft_new)(backward, dft_sizes[i], batch_size,
                                           x_stride_0, x_stride_1, 1, y_stride);
  } else {
    const Long n = dft_sizes[i];
    const Long s = internal_y_stride / n;
    return NS(ComplexFunctionPair_1_new)(
        NS(ComplexFunctionLoop_new)(
            NS_(TransposeDft_create)(data, backward, i - 1, dft_sizes,
                                     batch_size, x_stride_0 * n, x_stride_1,
                                     y_stride, s),
            n, x_stride_0, s),
        NS(ComplexFunctionLoop_new)(
            NS(CDftCore_TwiddleDft_new)(data, backward, n, s), batch_size,
            y_stride, y_stride));
  }
}

struct NS(ComplexFunction) *NS_(TransposeDft_new_)(
    struct NS(DataFactory) *data, bool backward, Long dimension,
    const int *dft_sizes, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride) {
#if PRINT_MODEL
  printf("CooleyTurkey_TransposeDft(%s", BOOL_STR(backward));
  for (Long i = 0; i < dimension; ++i) {
    printf(",%d", dft_sizes[i]);
  }
  printf(";%ld)\n", batch_size);
#endif
  const Long m = dimension;
  const Long dft_size = jomfftInt_ProductLong(m, dft_sizes);
  if (m == 1) {
    return NS(CDftCore_StrideBatchDft_new)(backward, dft_sizes[0], batch_size,
                                           x_stride_0, x_stride_1, 1, y_stride);
  } else {
    return NS_(TransposeDft_create)(data, backward, m - 1, dft_sizes,
                                    batch_size, x_stride_0, x_stride_1,
                                    y_stride, dft_size);
  }
}

struct NS(ComplexFunction) *NS_(TransposeDft_new)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride) {
  struct jomfftIntVector *dft_sizes = NS_(TransposeDft_Split)(dft_factors);
  struct NS(ComplexFunction) *out =
      NS_(TransposeDft_new_)(data, backward, jomfftIntVector_size(dft_sizes),
                             jomfftIntVector_data(dft_sizes), batch_size,
                             x_stride_0, x_stride_1, y_stride);
  jomfftIntVector_delete(dft_sizes);
  return out;
}
