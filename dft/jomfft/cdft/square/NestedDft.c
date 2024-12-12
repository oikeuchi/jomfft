#include "NestedDft.h"

#include "../../model/ComplexCopy.h"
#include "../../model/ComplexFunctionBufferedLoop.h"
#include "../../model/ComplexFunctionBufferedNestedLoop.h"
#include "../../model/ComplexFunctionBufferedTriple.h"
#include "../../model/ComplexFunctionLoop.h"
#include "../../model/ComplexFunctionPair.h"
#include "../../model/ComplexFunctionPairW.h"
#include "../../model/ComplexFunctionTriple.h"
#include "../../model/ComplexFunctionTripleW.h"
#include "../../model/ThreadingUtil.h"
#include "../cooleyturkey/Dft.h"
#include "../cooleyturkey/TransposeDft.h"
#include "../core/InnerBatchDft.h"
#include "../core/SquareSplitTwiddleDft.h"
#include "../core/TransposeDftTwiddle.h"
#include "SmallTransposeDft.h"

#define NS_(n) NS(CDftSquare_##n)

static struct NS(ComplexFunctionW) *NS_(NestedDft_Dft_newW)(
    struct NS(DataFactory) *data, bool backward, int square_dft_size,
    const struct jomfftPrimeFactors *inner_dft_factors, int num_threads) {
  const Long inner_dft_size = jomfftPrimeFactors_productLong(inner_dft_factors);
  if (square_dft_size * inner_dft_size >= C_L1_CACHE_SIZE &&
      NS_(NestedDft_CanSplit)(inner_dft_factors)) {
    return NS(ComplexFunctionPairW_new)(
        NS_(NestedDft_new)(data, backward, inner_dft_factors, square_dft_size,
                           1, false, num_threads),
        NS(Complex_Copy_new_)(inner_dft_size, 1, square_dft_size, num_threads),
        inner_dft_size * sizeof(Complex));
  } else {
    return NS(ComplexFunctionPairW_new)(
        NS(CDftCooleyTurkey_Dft_new)(data, backward, inner_dft_factors,
                                     square_dft_size, 1, false, num_threads),
        NS(Complex_Copy_new)(inner_dft_size, 1, square_dft_size),
        inner_dft_size * sizeof(Complex));
  }
}

static struct NS(ComplexFunction) *NS_(NestedDft_BatchDftLoop_new)(
    struct NS(DataFactory) *data, bool backward, int square_dft_size,
    const struct jomfftPrimeFactors *inner_dft_factors, int num_threads) {
  const Long inner_dft_size = jomfftPrimeFactors_productLong(inner_dft_factors);
  const Long stride_0 = square_dft_size * inner_dft_size;
  const Long stride_1 = square_dft_size;
  if (inner_dft_size <= (Long)INT_MAX &&
      NS(CDftCore_InnerBatchDft_HasKey)((int)inner_dft_size)) {
    if (square_dft_size > C_SIMD_SIZE && square_dft_size % C_SIMD_SIZE == 0) {
      return NS(CDftCore_NestedInnerBatchDft_new)(
          backward, (int)inner_dft_size, square_dft_size, square_dft_size,
          stride_0, stride_1, stride_0, stride_1, num_threads);
    } else {
      return NS(ComplexFunctionLoop_new)(
          NS(CDftCore_InnerBatchDft_new)(backward, (int)inner_dft_size,
                                         square_dft_size, stride_1, stride_1,
                                         1),
          square_dft_size, stride_0, stride_0);
    }
  } else {
    if (num_threads <= square_dft_size &&
        stride_0 <= C_L3_CACHE_SIZE / num_threads &&
        square_dft_size * inner_dft_size <= C_L1_CACHE_SIZE / 2) {
      if (NS_(SmallTransposeDft_CanSplit)(inner_dft_factors)) {
        return NS(ComplexFunctionBufferedLoop_new_)(
            NS(ComplexFunctionPairW_new)(
                NS_(SmallTransposeDft_new)(data, backward, inner_dft_factors,
                                           square_dft_size, stride_1, 1,
                                           inner_dft_size),
                NS(Complex_Copy2D_new)(inner_dft_size, square_dft_size, 1,
                                       inner_dft_size, stride_1, 1),
                square_dft_size * inner_dft_size * sizeof(Complex)),
            square_dft_size, stride_0, stride_0, num_threads);
      } else {
        return NS(ComplexFunctionBufferedLoop_new_)(
            NS(ComplexFunctionPairW_new)(
                NS(CDftCooleyTurkey_TransposeDft_new)(
                    data, backward, inner_dft_factors, square_dft_size,
                    stride_1, 1, inner_dft_size),
                NS(Complex_Copy2D_new)(inner_dft_size, square_dft_size, 1,
                                       inner_dft_size, stride_1, 1),
                square_dft_size * inner_dft_size * sizeof(Complex)),
            square_dft_size, stride_0, stride_0, num_threads);
      }
    } else {
      int num_threads_0 = 1;
      int num_threads_1 = 1;
      NS(Threading_Split)(num_threads, square_dft_size * square_dft_size, 1,
                          inner_dft_size, &num_threads_0, &num_threads_1);
      return NS(ComplexFunctionBufferedNestedLoop_new)(
          NS_(NestedDft_Dft_newW)(data, backward, square_dft_size,
                                  inner_dft_factors, num_threads_1),
          square_dft_size, square_dft_size, stride_0, 1, stride_0, 1,
          num_threads_0);
    }
  }
}

static void NS_(NestedDft_Split)(const struct jomfftPrimeFactors *dft_factors,
                                 int *square_dft_size,
                                 struct jomfftPrimeFactors *inner_dft_factors) {
  assert(square_dft_size);
  assert(inner_dft_factors);
  assert(NS_(NestedDft_CanSplit)(dft_factors));
  *inner_dft_factors = *dft_factors;
  const bool has_other_exponent =
      (dft_factors->exponents[1] + dft_factors->exponents[2] +
       dft_factors->exponents[3]) > 0;
  if (dft_factors->exponents[0] > 6 ||
      (dft_factors->exponents[0] == 6 && has_other_exponent)) {
    *square_dft_size = 8;
    inner_dft_factors->exponents[0] -= 6;
  } else if (dft_factors->exponents[0] > 4 ||
             (dft_factors->exponents[0] == 4 && has_other_exponent)) {
    *square_dft_size = 4;
    inner_dft_factors->exponents[0] -= 4;
  } else {
    *square_dft_size = 2;
    inner_dft_factors->exponents[0] -= 2;
  }
}

bool NS_(NestedDft_CanSplit)(const struct jomfftPrimeFactors *dft_factors) {
  const bool has_other_exponent =
      (dft_factors->exponents[1] + dft_factors->exponents[2] +
       dft_factors->exponents[3]) > 0;
  return dft_factors->exponents[0] > 2 ||
         (dft_factors->exponents[0] == 2 && has_other_exponent);
}

struct NS(ComplexFunction) *NS_(NestedDft_new_1)(
    struct NS(DataFactory) *data, bool backward, int square_dft_size,
    const struct jomfftPrimeFactors *inner_dft_factors, Long x_stride,
    int num_threads) {
  const Long inner_dft_size = jomfftPrimeFactors_productLong(inner_dft_factors);
#if PRINT_MODEL
  printf("Square_NestedDft(%s,%d,%ld;%d)\n", BOOL_STR(backward),
         square_dft_size, inner_dft_size, num_threads);
#endif
  return NS(ComplexFunctionTriple_0_new)(
      NS(CDftCore_TransposeDftTwiddle_new)(data, backward, square_dft_size,
                                           inner_dft_size, x_stride,
                                           num_threads),
      NS_(NestedDft_BatchDftLoop_new)(data, backward, square_dft_size,
                                      inner_dft_factors, num_threads),
      NS(CDftCore_SquareSplitTwiddleDft_new)(data, backward, square_dft_size,
                                             inner_dft_size, num_threads));
}

struct NS(ComplexFunction) *NS_(NestedDft_new)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long x_stride, Long y_stride,
    bool inplace, int num_threads) {
  int q;
  struct jomfftPrimeFactors r;
  NS_(NestedDft_Split)(dft_factors, &q, &r);
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  if (y_stride == 1) {
    return NS_(NestedDft_new_1)(data, backward, q, &r, x_stride, num_threads);
  } else if (!inplace && x_stride == 1) {
    return NS(ComplexFunctionPair_0_new)(
        NS_(NestedDft_new_1)(data, backward, q, &r, x_stride, num_threads),
        NS(Complex_Copy_new_)(dft_size, 1, y_stride, num_threads));
  } else {
    return NS(ComplexFunctionBufferedTriple_new)(
        NS(Complex_Copy_new_)(dft_size, x_stride, 1, num_threads),
        NS_(NestedDft_new_1)(data, backward, q, &r, 1, num_threads),
        NS(Complex_Copy_new_)(dft_size, 1, y_stride, num_threads),
        dft_size * sizeof(Complex));
  }
}
