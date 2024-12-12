#include "LargeBatchDft.h"

#include "../../model/ComplexFunctionLoop.h"
#include "../../model/ThreadingUtil.h"
#include "LargeDft.h"

#define NS_(n) NS(CDftSquare_##n)

bool NS_(LargeBatchDft_CanSplit)(const struct jomfftPrimeFactors *dft_factors) {
  return NS_(LargeDft_CanSplit)(dft_factors);
}

struct NS(ComplexFunction) *NS_(LargeBatchDft_new)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    bool inplace, int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  int num_threads_0 = 1;
  int num_threads_1 = 1;
  NS(Threading_Split)(num_threads, batch_size, 1, dft_size, &num_threads_0,
                      &num_threads_1);
  return NS(ComplexFunctionLoop_new_)(
      NS_(LargeDft_new)(data, backward, dft_factors, x_stride_0, y_stride_0,
                        inplace, num_threads_1),
      batch_size, x_stride_1, y_stride_1, num_threads_0);
}
