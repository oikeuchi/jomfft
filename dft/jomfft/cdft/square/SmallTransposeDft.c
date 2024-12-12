#include "SmallTransposeDft.h"

#include "../../model/ComplexFunctionLoop.h"
#include "../../model/ComplexFunctionPair.h"
#include "../core/SquareSplitTwiddleDft.h"
#include "../core/StrideBatchDft.h"
#include "../core/TwiddleDft.h"

#define NS_(n) NS(CDftSquare_##n)

static void NS_(SmallTransposeDft_Split)(
    const struct jomfftPrimeFactors *dft_factors, int *square_dft_size,
    int *inner_dft_size) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  if (dft_size >= 1024 && dft_size % 64 == 0 &&
      NS(CDftCore_TwiddleDft_HasKey)(dft_size / 64)) {
    *square_dft_size = 8;
    *inner_dft_size = dft_size / 64;
  } else if (dft_size > 16 && dft_size % 16 == 0 &&
             NS(CDftCore_TwiddleDft_HasKey)(dft_size / 16)) {
    *square_dft_size = 4;
    *inner_dft_size = dft_size / 16;
  } else {
    *square_dft_size = 0;
    *inner_dft_size = 0;
  }
}

bool NS_(SmallTransposeDft_CanSplit)(
    const struct jomfftPrimeFactors *dft_factors) {
  int s, q;
  NS_(SmallTransposeDft_Split)(dft_factors, &s, &q);
  return s > 0 && q > 0;
}

struct NS(ComplexFunction) *NS_(SmallTransposeDft_new_)(
    struct NS(DataFactory) *data, bool backward, int square_dft_size,
    int inner_dft_size, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride) {
#if PRINT_MODEL
  printf("Square_SmallTransposeDft(%s,%d,%d;%ld)\n", BOOL_STR(backward),
         square_dft_size, inner_dft_size, batch_size);
#endif
  const Long s0 = square_dft_size * inner_dft_size;
  const Long s1 = square_dft_size;
  if (batch_size == 1) {
    return NS(ComplexFunctionPair_1_new)(
        NS(ComplexFunctionLoop_new)(
            NS(ComplexFunctionPair_1_new)(
                NS(CDftCore_StrideBatchDft_new)(backward, square_dft_size,
                                                inner_dft_size, s0 * x_stride_0,
                                                s1 * x_stride_0, 1, s1),
                NS(CDftCore_TwiddleDft_new)(data, backward, inner_dft_size,
                                            square_dft_size)),
            square_dft_size, x_stride_0, s0),
        NS(CDftCore_SquareSplitTwiddleDft_new)(data, backward, square_dft_size,
                                               inner_dft_size, 1));
  } else {
    return NS(ComplexFunctionPair_1_new)(
        NS(ComplexFunctionLoop_new)(
            NS(ComplexFunctionPair_1_new)(
                NS(ComplexFunctionLoop_new)(
                    NS(CDftCore_StrideBatchDft_new)(backward, square_dft_size,
                                                    batch_size, s0 * x_stride_0,
                                                    x_stride_1, 1, y_stride),
                    inner_dft_size, s1 * x_stride_0, s1),
                NS(ComplexFunctionLoop_new)(
                    NS(CDftCore_TwiddleDft_new)(data, backward, inner_dft_size,
                                                square_dft_size),
                    batch_size, y_stride, y_stride)),
            square_dft_size, x_stride_0, s0),
        NS(ComplexFunctionLoop_new)(
            NS(CDftCore_SquareSplitTwiddleDft_new)(
                data, backward, square_dft_size, inner_dft_size, 1),
            batch_size, y_stride, y_stride));
  }
}

struct NS(ComplexFunction) *NS_(SmallTransposeDft_new)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride) {
  int s, q;
  NS_(SmallTransposeDft_Split)(dft_factors, &s, &q);
  if (s > 0 && q > 0) {
    return NS_(SmallTransposeDft_new_)(data, backward, s, q, batch_size,
                                       x_stride_0, x_stride_1, y_stride);
  } else {
    return NULL;
  }
}
