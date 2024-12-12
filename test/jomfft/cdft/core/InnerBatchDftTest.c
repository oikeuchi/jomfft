#include "../InnerBatchDftTest.h"

#include <jomfft/cdft/core/InnerBatchDft.h>
#include <jomfft/util/random/ComplexTensor.h>

#include "../naive/BatchDft.h"
#include "InnerBatchDftTest.h"

static void CDftCore_InnerBatchDftTest__(bool backward, int dft_size,
                                         Long batch_size, int num_threads) {
  CAPTURE_INT(num_threads);
  struct NS(ComplexFunction) *p = NS(CDftCore_InnerBatchDft_new)(
      backward, dft_size, batch_size, batch_size, batch_size, num_threads);
  NS(CDft_InnerBatchDftTest)(p, backward, dft_size, batch_size, false);
  NS(ComplexFunction_delete)(p);
}

static void CDftCore_InnerBatchDftTest_(bool backward, int dft_size,
                                        Long batch_size) {
  CDftCore_InnerBatchDftTest__(backward, dft_size, batch_size, 1);
}

void NS(CDftCore_InnerBatchDftTest)() {
  CDftCore_InnerBatchDftTest_(false, 2, 1);
  CDftCore_InnerBatchDftTest_(false, 4, 2);
  CDftCore_InnerBatchDftTest_(true, 8, 3);
  CDftCore_InnerBatchDftTest_(true, 16, 4);
  CDftCore_InnerBatchDftTest_(false, 2, 5);
#if USE_OPENMP
  CDftCore_InnerBatchDftTest__(false, 4, 8, 2);
  CDftCore_InnerBatchDftTest__(false, 4, 9, 2);
  CDftCore_InnerBatchDftTest__(false, 4, 10, 2);
  CDftCore_InnerBatchDftTest__(false, 4, 11, 2);
  CDftCore_InnerBatchDftTest__(false, 4, 12, 2);
  CDftCore_InnerBatchDftTest__(true, 4, 8, 2);
  CDftCore_InnerBatchDftTest__(true, 4, 9, 2);
  CDftCore_InnerBatchDftTest__(true, 4, 10, 2);
  CDftCore_InnerBatchDftTest__(true, 4, 11, 2);
  CDftCore_InnerBatchDftTest__(true, 4, 12, 2);
#endif
}

static void CDftCore_NestedInnerBatchDftTest__(bool backward, int dft_size,
                                               Long batch_size_0,
                                               Long batch_size_1,
                                               int num_threads) {
  CAPTURE_BOOL(backward);
  CAPTURE_INT(dft_size);
  CAPTURE_LONG(batch_size_0);
  CAPTURE_LONG(batch_size_1);
  CAPTURE_INT(num_threads);
  const Long stride_0 = dft_size * batch_size_1;
  const Long stride_1 = batch_size_1;
  struct NS(ComplexTensor) *x =
      NS(Random_ComplexTensor_new3)(batch_size_0, dft_size, batch_size_1);
  struct NS(ComplexTensor) *x0 = NS(ComplexTensor_copy)(x);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_zerosLike)(x);
  struct NS(ComplexFunction) *func = NS(CDftCore_NestedInnerBatchDft_new)(
      backward, dft_size, batch_size_0, batch_size_1, stride_0, stride_1,
      stride_0, stride_1, num_threads);
  NS(ComplexFunction_evaluate)(func, x0->data, y0->data);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(x);
  for (Long i = 0; i < batch_size_0; ++i) {
    NS(CDftNaive_BatchDft)(backward, dft_size, batch_size_1,
                           NS(ComplexTensor_at3)(x, i, 0, 0), stride_1, 1,
                           NS(ComplexTensor_at3)(y1, i, 0, 0), stride_1, 1);
  }
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
  NS(ComplexFunction_delete)(func);
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(x0);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
}

static void CDftCore_NestedInnerBatchDftTest_(bool backward, int dft_size,
                                              Long batch_size_0,
                                              Long batch_size_1) {
  CDftCore_NestedInnerBatchDftTest__(backward, dft_size, batch_size_0,
                                     batch_size_1, 1);
}

void NS(CDftCore_NestedInnerBatchDftTest)() {
  CDftCore_NestedInnerBatchDftTest_(false, 2, 4, C_SIMD_SIZE);
  CDftCore_NestedInnerBatchDftTest_(true, 4, 8, 2 * C_SIMD_SIZE);
  CDftCore_NestedInnerBatchDftTest_(false, 8, 8, C_SIMD_SIZE);
#if USE_OPENMP
  CDftCore_NestedInnerBatchDftTest__(false, 2, 4, C_SIMD_SIZE, 2);
  CDftCore_NestedInnerBatchDftTest__(true, 4, 8, 2 * C_SIMD_SIZE, 2);
  CDftCore_NestedInnerBatchDftTest__(false, 8, 8, C_SIMD_SIZE, 2);
  CDftCore_NestedInnerBatchDftTest__(false, 8, 8, 2 * C_SIMD_SIZE,
                                     omp_get_max_threads());
  CDftCore_NestedInnerBatchDftTest__(true, 4, 8, C_SIMD_SIZE,
                                     omp_get_max_threads());
#endif
}
