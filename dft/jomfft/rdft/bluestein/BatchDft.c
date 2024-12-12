#include "BatchDft.h"

#include "../../cdft/bluestein/Dft.h"
#include "../../model/ComplexCopy.h"
#include "../../model/ComplexToRealFunctionBuffered.h"
#include "../../model/ComplexToRealFunctionBufferedLoop.h"
#include "../../model/ComplexToRealFunctionTripleW.h"
#include "../../model/RealCopy.h"
#include "../../model/RealToComplexFunctionBuffered.h"
#include "../../model/RealToComplexFunctionBufferedLoop.h"
#include "../../model/RealToComplexFunctionTripleW.h"
#include "../../model/SplitComplexCopy.h"
#include "../../model/ThreadingUtil.h"
#include "../core/ComplexToPairDft.h"
#include "Dft.h"

#define NS_(n) NS(RDftBluestein_##n)

static struct NS(RealToComplexFunctionW) *NS_(PairDft_F_newW)(
    struct NS(DataFactory) *data, Long dft_size, Long x_stride_0,
    Long x_stride_1, Long y_stride_0, Long y_stride_1, int num_threads) {
  return NS(RealToComplexFunctionTripleWB_0_new)(
      NS(SplitComplex_Copy_F_new)(dft_size, x_stride_0, 1, x_stride_1,
                                  num_threads),
      NS(CDftBluestein_Dft_newW)(data, false, dft_size, 1, 1, num_threads),
      NS(RDftCore_ComplexToPairDft_F_new)(dft_size, y_stride_0, y_stride_1,
                                          num_threads));
}

static struct NS(ComplexToRealFunctionW) *NS_(PairDft_B_newW)(
    struct NS(DataFactory) *data, Long dft_size, Long x_stride_0,
    Long x_stride_1, Long y_stride_0, Long y_stride_1, int num_threads) {
  return NS(ComplexToRealFunctionTripleWB_2_new)(
      NS(RDftCore_ComplexToPairDft_B_new)(dft_size, x_stride_0, x_stride_1,
                                          num_threads),
      NS(CDftBluestein_Dft_newW)(data, true, dft_size, 1, 1, num_threads),
      NS(SplitComplex_Copy_B_new)(dft_size, 1, y_stride_0, y_stride_1,
                                  num_threads));
}

struct NS(RealToComplexFunction) *NS_(BatchDft_F_new)(
    struct NS(DataFactory) *data, Long dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
  struct NS(RealToComplexFunction) *out = NULL;
  if (batch_size == 1) {
    out = NS(RealToComplexFunctionBuffered_new)(
        NS_(Dft_F_newW)(data, dft_size, x_stride_0, y_stride_0, num_threads));
  } else {
    int num_threads_0 = 1;
    int num_threads_1 = 1;
    NS(Threading_Split_)
    (num_threads, (batch_size + 1) / 2, 1, dft_size, &num_threads_0,
     &num_threads_1, true);
    if (batch_size % 2 == 0) {
      out = NS(RealToComplexFunctionBufferedLoop_new_)(
          NS_(PairDft_F_newW)(data, dft_size, x_stride_0, x_stride_1,
                              y_stride_0, y_stride_1, num_threads_1),
          batch_size / 2, x_stride_1 * 2, y_stride_1 * 2, num_threads_0);
    } else {
      out = NS(RealToComplexFunctionBufferedLoopR_new_)(
          NS_(PairDft_F_newW)(data, dft_size, x_stride_0, x_stride_1,
                              y_stride_0, y_stride_1, num_threads_1),
          NS_(Dft_F_newW)(data, dft_size, x_stride_0, y_stride_0,
                          num_threads_1),
          batch_size / 2, x_stride_1 * 2, y_stride_1 * 2, num_threads_0);
    }
  }
  return out;
}

struct NS(ComplexToRealFunction) *NS_(BatchDft_B_new)(
    struct NS(DataFactory) *data, Long dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
  struct NS(ComplexToRealFunction) *out = NULL;
  if (batch_size == 1) {
    out = NS(ComplexToRealFunctionBuffered_new)(
        NS_(Dft_B_newW)(data, dft_size, x_stride_0, y_stride_0, num_threads));
  } else {
    int num_threads_0 = 1;
    int num_threads_1 = 1;
    NS(Threading_Split_)
    (num_threads, (batch_size + 1) / 2, 1, dft_size, &num_threads_0,
     &num_threads_1, true);
    if (batch_size % 2 == 0) {
      out = NS(ComplexToRealFunctionBufferedLoop_new_)(
          NS_(PairDft_B_newW)(data, dft_size, x_stride_0, x_stride_1,
                              y_stride_0, y_stride_1, num_threads_1),
          batch_size / 2, x_stride_1 * 2, y_stride_1 * 2, num_threads_0);
    } else {
      out = NS(ComplexToRealFunctionBufferedLoopR_new_)(
          NS_(PairDft_B_newW)(data, dft_size, x_stride_0, x_stride_1,
                              y_stride_0, y_stride_1, num_threads_1),
          NS_(Dft_B_newW)(data, dft_size, x_stride_0, y_stride_0,
                          num_threads_1),
          batch_size / 2, x_stride_1 * 2, y_stride_1 * 2, num_threads_0);
    }
  }
  return out;
}
