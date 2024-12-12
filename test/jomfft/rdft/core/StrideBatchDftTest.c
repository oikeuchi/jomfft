#include "StrideBatchDftTest.h"

#include <jomfft/rdft/core/StrideBatchDft.h>

#include "../BatchDftTest_.h"
#include "../OuterBatchDftTest.h"

static void RDftCore_StrideBatchDft_FTestNotInplace(int dft_size,
                                                    Long batch_size,
                                                    Long stride,
                                                    int num_threads) {
  CAPTURE_INT(num_threads);
  const Long y_size = CE_SIZE(dft_size);
  const Long x_stride_1 = dft_size * stride;
  const Long y_stride_1 = y_size * stride;
  struct NS(RealToComplexFunction) *p = NS(RDftCore_StrideBatchDft_F_new)(
      dft_size, batch_size, stride, x_stride_1, stride, y_stride_1,
      num_threads);
  NS(RDft_OuterBatchDft_FTest)(p, dft_size, batch_size, stride, stride, false);
  NS(RealToComplexFunction_delete)(p);
  NS(Test_ClearCapture)();
}

static void RDftCore_StrideBatchDft_FTestInplace(int dft_size, Long batch_size,
                                                 int num_threads) {
  CAPTURE_INT(num_threads);
  const Long y_size = CE_SIZE(dft_size);
  const Long x_size = 2 * y_size;
  struct NS(RealToComplexFunction) *p = NS(RDftCore_StrideBatchDft_F_new)(
      dft_size, batch_size, 1, x_size, 1, y_size, num_threads);
  NS(RDft_OuterBatchDft_FTest)(p, dft_size, batch_size, 1, 1, true);
  NS(RealToComplexFunction_delete)(p);
  NS(Test_ClearCapture)();
}

static void RDftCore_StrideBatchDft_FTest__(int dft_size, Long batch_size,
                                            int num_threads) {
  RDftCore_StrideBatchDft_FTestNotInplace(dft_size, batch_size, 1, num_threads);
  RDftCore_StrideBatchDft_FTestNotInplace(dft_size, batch_size, 2, num_threads);
  RDftCore_StrideBatchDft_FTestInplace(dft_size, batch_size, num_threads);
}

static void RDftCore_StrideBatchDft_FTest_(int dft_size, Long batch_size) {
  RDftCore_StrideBatchDft_FTest__(dft_size, batch_size, 1);
}

void NS(RDftCore_StrideBatchDft_F_1Test)() {
  RDftCore_StrideBatchDft_FTest_(2, 1);
  RDftCore_StrideBatchDft_FTest_(8, 3);
  RDftCore_StrideBatchDft_FTest_(4, 16);
#if USE_OPENMP
  RDftCore_StrideBatchDft_FTest__(4, 16, 2);
  RDftCore_StrideBatchDft_FTest__(4, 15, 2);
#endif
}

static void RDftCore_StrideBatchDft_BTestNotInplace(int dft_size,
                                                    Long batch_size,
                                                    Long stride,
                                                    int num_threads) {
  CAPTURE_INT(num_threads);
  const Long x_size = CE_SIZE(dft_size);
  const Long x_stride_1 = x_size * stride;
  const Long y_stride_1 = dft_size * stride;
  struct NS(ComplexToRealFunction) *p = NS(RDftCore_StrideBatchDft_B_new)(
      dft_size, batch_size, stride, x_stride_1, stride, y_stride_1,
      num_threads);
  NS(RDft_OuterBatchDft_BTest)(p, dft_size, batch_size, stride, stride, false);
  NS(ComplexToRealFunction_delete)(p);
  NS(Test_ClearCapture)();
}

static void RDftCore_StrideBatchDft_BTestInplace(int dft_size, Long batch_size,
                                                 int num_threads) {
  CAPTURE_INT(num_threads);
  const Long x_size = CE_SIZE(dft_size);
  const Long y_size = 2 * x_size;
  struct NS(ComplexToRealFunction) *p = NS(RDftCore_StrideBatchDft_B_new)(
      dft_size, batch_size, 1, x_size, 1, y_size, num_threads);
  NS(RDft_OuterBatchDft_BTest)(p, dft_size, batch_size, 1, 1, true);
  NS(ComplexToRealFunction_delete)(p);
  NS(Test_ClearCapture)();
}

static void RDftCore_StrideBatchDft_BTest__(int dft_size, Long batch_size,
                                            int num_threads) {
  RDftCore_StrideBatchDft_BTestNotInplace(dft_size, batch_size, 1, num_threads);
  RDftCore_StrideBatchDft_BTestNotInplace(dft_size, batch_size, 2, num_threads);
  RDftCore_StrideBatchDft_BTestInplace(dft_size, batch_size, num_threads);
}

static void RDftCore_StrideBatchDft_BTest_(int dft_size, Long batch_size) {
  RDftCore_StrideBatchDft_BTest__(dft_size, batch_size, 1);
}

void NS(RDftCore_StrideBatchDft_B_1Test)() {
  RDftCore_StrideBatchDft_BTest_(2, 1);
  RDftCore_StrideBatchDft_BTest_(8, 3);
  RDftCore_StrideBatchDft_BTest_(4, 16);
#if USE_OPENMP
  RDftCore_StrideBatchDft_BTest__(4, 16, 2);
  RDftCore_StrideBatchDft_BTest__(4, 15, 2);
#endif
}

static struct NS(RealToComplexFunction) *
RDftCore_StrideBatchDftFactory_F_create(struct NS(RDft_BatchDftFactory_F) *p,
                                        struct NS(DataFactory) *data,
                                        Long dft_size, Long batch_size,
                                        Long x_stride_0, Long x_stride_1,
                                        Long y_stride_0, Long y_stride_1,
                                        bool inplace, int num_threads) {
  (void)p;
  (void)data;
  (void)inplace;
  return NS(RDftCore_StrideBatchDft_F_new)((int)dft_size, batch_size,
                                           x_stride_0, x_stride_1, y_stride_0,
                                           y_stride_1, num_threads);
}

void NS(RDftCore_StrideBatchDft_FTest)() {
  struct NS(RDft_BatchDftFactory_F) *factory = NS(RDft_BatchDftFactory_F_new)(
      RDftCore_StrideBatchDftFactory_F_create, NULL, NULL);
  NS(RDft_BatchDft_FTest_)(factory, 4);
  NS(RDft_BatchDftFactory_F_delete)(factory);
}

static struct NS(ComplexToRealFunction) *
RDftCore_StrideBatchDftFactory_B_create(struct NS(RDft_BatchDftFactory_B) *p,
                                        struct NS(DataFactory) *data,
                                        Long dft_size, Long batch_size,
                                        Long x_stride_0, Long x_stride_1,
                                        Long y_stride_0, Long y_stride_1,
                                        bool inplace, int num_threads) {
  (void)p;
  (void)data;
  (void)inplace;
  return NS(RDftCore_StrideBatchDft_B_new)((int)dft_size, batch_size,
                                           x_stride_0, x_stride_1, y_stride_0,
                                           y_stride_1, num_threads);
}

void NS(RDftCore_StrideBatchDft_BTest)() {
  struct NS(RDft_BatchDftFactory_B) *factory = NS(RDft_BatchDftFactory_B_new)(
      RDftCore_StrideBatchDftFactory_B_create, NULL, NULL);
  NS(RDft_BatchDft_BTest_)(factory, 4);
  NS(RDft_BatchDftFactory_B_delete)(factory);
}
