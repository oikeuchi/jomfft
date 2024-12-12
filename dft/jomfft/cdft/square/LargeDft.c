#include "LargeDft.h"

#include "../../model/ComplexFunctionBufferedTriple.h"
#include "../../model/ComplexFunctionLoop.h"
#include "../../model/ComplexFunctionPair.h"
#include "../../model/ComplexFunctionTriple.h"
#include "../../model/ComplexTranspose.h"
#include "../BatchDft.h"
#include "BlockSize.h"
#include "PairTwiddleTranspose.h"
#include "TwiddleTranspose.h"

#define NS_(n) NS(CDftSquare_##n)

static struct NS(ComplexFunction) *NS_(LargeDft_BatchDft_0_new)(
    struct NS(DataFactory) *data, bool backward, Long square_dft_size,
    Long x_stride, Long internal_stride, bool inplace, int num_threads) {
  if (inplace && x_stride == 1) {
    return NS(ComplexFunctionPair_0_new)(
        NS(Complex_Transpose_new_)(square_dft_size, internal_stride,
                                   num_threads),
        NS(CDft_BatchDft_new)(data, backward, square_dft_size, square_dft_size,
                              1, internal_stride, 1, internal_stride, true,
                              num_threads));
  } else {
    return NS(CDft_BatchDft_new)(data, backward, square_dft_size,
                                 square_dft_size, x_stride * internal_stride,
                                 x_stride, 1, internal_stride, false,
                                 num_threads);
  }
}

static struct NS(ComplexFunction) *NS_(LargeDft_BatchDft_1_new)(
    struct NS(DataFactory) *data, bool backward, Long square_dft_size,
    Long y_stride, Long internal_stride, bool inplace, int num_threads) {
  if (inplace && y_stride == 1) {
    return NS(ComplexFunctionPair_0_new)(
        NS(CDft_BatchDft_new)(data, backward, square_dft_size, square_dft_size,
                              1, internal_stride, 1, internal_stride, true,
                              num_threads),
        NS(Complex_Transpose_new_)(square_dft_size, internal_stride,
                                   num_threads));
  } else {
    return NS(CDft_BatchDft_new)(
        data, backward, square_dft_size, square_dft_size, 1, internal_stride,
        y_stride * internal_stride, y_stride, false, num_threads);
  }
}

static void NS_(LargeDft_Split)(const struct jomfftPrimeFactors *dft_factors,
                                Long *square_dft_size, int *inner_dft_size) {
  assert(square_dft_size);
  assert(inner_dft_size);
  assert(NS_(LargeDft_CanSplit)(dft_factors));
  Long r;
  jomfftPrimeFactors_sqrtLong(dft_factors, square_dft_size, &r);
  *inner_dft_size = (int)r;
}

bool NS_(LargeDft_CanSplit)(const struct jomfftPrimeFactors *dft_factors) {
  Long q, r;
  jomfftPrimeFactors_sqrtLong(dft_factors, &q, &r);
  return q > 1 && q % BLOCK_SIZE == 0 && (r == 1 || r == 2);
}

struct NS(ComplexFunction) *NS_(LargeDft_new_1)(struct NS(DataFactory) *data,
                                                bool backward,
                                                Long square_dft_size,
                                                int inner_dft_size,
                                                Long x_stride, Long y_stride,
                                                bool inplace, int num_threads) {
  const Long internal_stride = square_dft_size * inner_dft_size;
#if PRINT_MODEL
  printf("Square_LargeDft(%s,%ld,%d;%d)\n", BOOL_STR(backward), square_dft_size,
         inner_dft_size, num_threads);
#endif
  struct NS(ComplexFunction) *func0 = NULL;
  struct NS(ComplexFunction) *func1 = NULL;
  struct NS(ComplexFunction) *func2 = NULL;
  const bool use_input_as_work = x_stride == 1 && y_stride != 1;
  const bool use_output_as_work = y_stride == 1;
  func0 = NS_(LargeDft_BatchDft_0_new)(
      data, backward, square_dft_size, x_stride, internal_stride,
      inplace || use_input_as_work, num_threads);
  func2 = NS_(LargeDft_BatchDft_1_new)(
      data, backward, square_dft_size, y_stride, internal_stride,
      inplace || use_output_as_work, num_threads);
  if (inner_dft_size == 1) {
    func1 =
        NS_(TwiddleTranspose_new)(data, backward, square_dft_size, num_threads);
  } else {
    func0 = NS(ComplexFunctionLoop_new)(func0, 2, x_stride * square_dft_size,
                                        square_dft_size);
    func1 = NS_(PairTwiddleTranspose_new)(data, backward, square_dft_size,
                                          num_threads);
    func2 = NS(ComplexFunctionLoop_new)(func2, 2, square_dft_size,
                                        y_stride * square_dft_size);
  }
  if (use_input_as_work) {
    return NS(ComplexFunctionTriple_2_new)(func0, func1, func2);
  } else if (use_output_as_work) {
    return NS(ComplexFunctionTriple_0_new)(func0, func1, func2);
  } else {
    const Long dft_size = square_dft_size * square_dft_size * inner_dft_size;
    return NS(ComplexFunctionBufferedTriple_new)(func0, func1, func2,
                                                 dft_size * sizeof(Complex));
  }
}

struct NS(ComplexFunction) *NS_(LargeDft_new)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long x_stride, Long y_stride,
    bool inplace, int num_threads) {
  Long q;
  int r;
  NS_(LargeDft_Split)(dft_factors, &q, &r);
  return NS_(LargeDft_new_1)(data, backward, q, r, x_stride, y_stride, inplace,
                             num_threads);
}
