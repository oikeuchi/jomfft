#include "StrideBatchDftTest.h"

#include <jomfft/rdft/singleton/StrideBatchDft.h>

#include "../BatchDftTest_.h"
#include "../OuterBatchDftTest.h"

static void RDftSingleton_StrideBatchDft_FTestNotInplace(int dft_size,
                                                         Long batch_size,
                                                         Long stride,
                                                         int num_threads) {
  CAPTURE_INT(num_threads);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  const Long y_size = CE_SIZE(dft_size);
  const Long x_stride_1 = dft_size * stride;
  const Long y_stride_1 = y_size * stride;
  struct NS(RealToComplexFunction) *p = NS(RDftSingleton_StrideBatchDft_F_new)(
      data, dft_size, batch_size, stride, x_stride_1, stride, y_stride_1,
      num_threads);
  NS(RDft_OuterBatchDft_FTest)(p, dft_size, batch_size, stride, stride, false);
  NS(RealToComplexFunction_delete)(p);
  NS(DataFactory_delete)(data);
}

static void RDftSingleton_StrideBatchDft_FTestInplace(int dft_size,
                                                      Long batch_size,
                                                      int num_threads) {
  CAPTURE_INT(num_threads);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  const Long y_size = CE_SIZE(dft_size);
  const Long x_size = 2 * y_size;
  struct NS(RealToComplexFunction) *p = NS(RDftSingleton_StrideBatchDft_F_new)(
      data, dft_size, batch_size, 1, x_size, 1, y_size, num_threads);
  NS(RDft_OuterBatchDft_FTest)(p, dft_size, batch_size, 1, 1, true);
  NS(RealToComplexFunction_delete)(p);
  NS(DataFactory_delete)(data);
}

static void RDftSingleton_StrideBatchDft_FTest__(int dft_size, Long batch_size,
                                                 int num_threads) {
  RDftSingleton_StrideBatchDft_FTestNotInplace(dft_size, batch_size, 1,
                                               num_threads);
  RDftSingleton_StrideBatchDft_FTestNotInplace(dft_size, batch_size, 2,
                                               num_threads);
  RDftSingleton_StrideBatchDft_FTestInplace(dft_size, batch_size, num_threads);
}

static void RDftSingleton_StrideBatchDft_FTest_(int dft_size, Long batch_size) {
  RDftSingleton_StrideBatchDft_FTest__(dft_size, batch_size, 1);
}

void NS(RDftSingleton_StrideBatchDft_F_1Test)() {
  RDftSingleton_StrideBatchDft_FTest_(8, 1);
  RDftSingleton_StrideBatchDft_FTest_(11, 3);
  RDftSingleton_StrideBatchDft_FTest_(13, 16);
#if USE_OPENMP
  RDftSingleton_StrideBatchDft_FTest__(11, 16, 2);
  RDftSingleton_StrideBatchDft_FTest__(11, 15, 2);
#endif
}

static void RDftSingleton_StrideBatchDft_BTestNotInplace(int dft_size,
                                                         Long batch_size,
                                                         Long stride,
                                                         int num_threads) {
  CAPTURE_INT(num_threads);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  const Long x_size = CE_SIZE(dft_size);
  const Long x_stride_1 = x_size * stride;
  const Long y_stride_1 = dft_size * stride;
  struct NS(ComplexToRealFunction) *p = NS(RDftSingleton_StrideBatchDft_B_new)(
      data, dft_size, batch_size, stride, x_stride_1, stride, y_stride_1,
      num_threads);
  NS(RDft_OuterBatchDft_BTest)
  (p, dft_size, batch_size, stride, stride, false);
  NS(ComplexToRealFunction_delete)(p);
  NS(DataFactory_delete)(data);
}

static void RDftSingleton_StrideBatchDft_BTestInplace(int dft_size,
                                                      Long batch_size,
                                                      int num_threads) {
  CAPTURE_INT(num_threads);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  const Long x_size = CE_SIZE(dft_size);
  const Long y_size = 2 * x_size;
  struct NS(ComplexToRealFunction) *p = NS(RDftSingleton_StrideBatchDft_B_new)(
      data, dft_size, batch_size, 1, x_size, 1, y_size, num_threads);
  NS(RDft_OuterBatchDft_BTest)(p, dft_size, batch_size, 1, 1, true);
  NS(ComplexToRealFunction_delete)(p);
  NS(DataFactory_delete)(data);
}

static void RDftSingleton_StrideBatchDft_BTest__(int dft_size, Long batch_size,
                                                 int num_threads) {
  RDftSingleton_StrideBatchDft_BTestNotInplace(dft_size, batch_size, 1,
                                               num_threads);
  RDftSingleton_StrideBatchDft_BTestNotInplace(dft_size, batch_size, 2,
                                               num_threads);
  RDftSingleton_StrideBatchDft_BTestInplace(dft_size, batch_size, num_threads);
}

static void RDftSingleton_StrideBatchDft_BTest_(int dft_size, Long batch_size) {
  RDftSingleton_StrideBatchDft_BTest__(dft_size, batch_size, 1);
}

void NS(RDftSingleton_StrideBatchDft_B_1Test)() {
  RDftSingleton_StrideBatchDft_BTest_(8, 1);
  RDftSingleton_StrideBatchDft_BTest_(11, 3);
  RDftSingleton_StrideBatchDft_BTest_(13, 16);
#if USE_OPENMP
  RDftSingleton_StrideBatchDft_BTest__(11, 16, 2);
  RDftSingleton_StrideBatchDft_BTest__(11, 15, 2);
#endif
}

static struct NS(RealToComplexFunction) *
RDftSingleton_StrideBatchDftFactory_F_create(
    struct NS(RDft_BatchDftFactory_F) *p, struct NS(DataFactory) *data,
    Long dft_size, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride_0, Long y_stride_1, bool inplace, int num_threads) {
  (void)p;
  (void)inplace;
  return NS(RDftSingleton_StrideBatchDft_F_new)(
      data, (int)dft_size, batch_size, x_stride_0, x_stride_1, y_stride_0,
      y_stride_1, num_threads);
}

void NS(RDftSingleton_StrideBatchDft_FTest)() {
  struct NS(RDft_BatchDftFactory_F) *factory = NS(RDft_BatchDftFactory_F_new)(
      RDftSingleton_StrideBatchDftFactory_F_create, NULL, NULL);
  NS(RDft_BatchDft_FTest_)(factory, 11);
  NS(RDft_BatchDftFactory_F_delete)(factory);
}

static struct NS(ComplexToRealFunction) *
RDftSingleton_StrideBatchDftFactory_B_create(
    struct NS(RDft_BatchDftFactory_B) *p, struct NS(DataFactory) *data,
    Long dft_size, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride_0, Long y_stride_1, bool inplace, int num_threads) {
  (void)p;
  (void)inplace;
  return NS(RDftSingleton_StrideBatchDft_B_new)(
      data, (int)dft_size, batch_size, x_stride_0, x_stride_1, y_stride_0,
      y_stride_1, num_threads);
}

void NS(RDftSingleton_StrideBatchDft_BTest)() {
  struct NS(RDft_BatchDftFactory_B) *factory = NS(RDft_BatchDftFactory_B_new)(
      RDftSingleton_StrideBatchDftFactory_B_create, NULL, NULL);
  NS(RDft_BatchDft_BTest_)(factory, 11);
  NS(RDft_BatchDftFactory_B_delete)(factory);
}
