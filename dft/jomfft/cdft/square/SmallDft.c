#include "SmallDft.h"

#include "../../model/ComplexFunctionLoop.h"
#include "../../model/ComplexFunctionPair.h"
#include "../../model/ComplexTranspose.h"
#include "../cooleyturkey/Dft.h"
#include "../core/PairSquareTwiddleDft.h"
#include "../core/StrideBatchDft.h"
#include "../core/TwiddleDft.h"

#define DFT_SMALL_SIZE (C_L1_CACHE_SIZE / 4)

#define NS_(n) NS(CDftSquare_##n)

bool NS_(SmallDft_CanSplit)(const struct jomfftPrimeFactors *dft_factors) {
  Long q_, r_;
  jomfftPrimeFactors_sqrtLong(dft_factors, &q_, &r_);
  if (q_ * q_ * r_ > DFT_SMALL_SIZE || r_ > 2) {
    return false;
  }
  const int q = (int)q_;
  const int r = (int)r_;
  if (r == 1) {
    return NS(CDftCore_StrideBatchDft_HasKey)(q) &&
           NS(CDftCore_TwiddleDft_HasKey)(q);
  } else if (r == 2) {
    return NS(CDftCore_StrideBatchDft_HasKey)(q) &&
           NS(CDftCore_PairSquareTwiddleDft_HasKey)(q);
  } else {
    return false;
  }
}

void NS_(SmallDft_Split)(const struct jomfftPrimeFactors *dft_factors,
                         int *square_dft_size, int *inner_dft_size) {
  assert(square_dft_size);
  assert(inner_dft_size);
  assert(NS_(SmallDft_CanSplit)(dft_factors));
  Long q, r;
  jomfftPrimeFactors_sqrtLong(dft_factors, &q, &r);
  *square_dft_size = (int)q;
  *inner_dft_size = (int)r;
}

struct NS(ComplexFunction) *NS_(SmallDft_new_1)(struct NS(DataFactory) *data,
                                                bool backward,
                                                int square_dft_size,
                                                int inner_dft_size) {
#if PRINT_MODEL
  printf("Square_SmallDft(%s,%d,%d)\n", BOOL_STR(backward), square_dft_size,
         inner_dft_size);
#endif
  const Long stride = square_dft_size * inner_dft_size;
  struct NS(ComplexFunction) *dft = NS(ComplexFunctionPair_0_new)(
      NS(Complex_Transpose_new)(square_dft_size, stride),
      NS(CDftCore_StrideBatchDft_new)(backward, square_dft_size,
                                      square_dft_size, 1, stride, 1, stride));
  if (inner_dft_size == 1) {
    return NS(ComplexFunctionPair_1_new)(
        dft, NS(CDftCore_TwiddleDft_new)(data, backward, square_dft_size,
                                         square_dft_size));
  } else {
    assert(inner_dft_size == 2);
    return NS(ComplexFunctionPair_1_new)(
        NS(ComplexFunctionLoop_new)(dft, 2, square_dft_size, square_dft_size),
        NS(CDftCore_PairSquareTwiddleDft_new)(data, backward, square_dft_size));
  }
}

struct NS(ComplexFunction) *NS_(SmallDft_new)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long x_stride, Long y_stride,
    bool inplace) {
  int q, r;
  NS_(SmallDft_Split)(dft_factors, &q, &r);
  if (inplace && x_stride == 1 && y_stride == 1) {
    return NS_(SmallDft_new_1)(data, backward, q, r);
  } else {
    return NS(CDftCooleyTurkey_Dft_new)(data, backward, dft_factors, x_stride,
                                        y_stride, inplace, 1);
  }
}
