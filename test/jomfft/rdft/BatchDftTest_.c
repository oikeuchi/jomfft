#include "BatchDftTest_.h"
#include "InnerBatchDftTest.h"
#include "OuterBatchDftTest.h"
#include "TransposeDftTest.h"

#define NS_(n) NS(RDft_##n)

struct NS_(BatchDftFactory_F) {
  NS_(BatchDftFactory_F_fn) create;
  jomfftDestruct_fn destruct;
  void *data;
};

struct NS_(BatchDftFactory_F) *NS_(BatchDftFactory_F_new)(
    NS_(BatchDftFactory_F_fn) create, jomfftDestruct_fn destruct, void *data) {
  struct NS_(BatchDftFactory_F) *out = NEW(struct NS_(BatchDftFactory_F));
  *out = (struct NS_(BatchDftFactory_F)){
      .create = create, .destruct = destruct, .data = data};
  return out;
}

void NS_(BatchDftFactory_F_delete)(struct NS_(BatchDftFactory_F) *p) {
  if (p) {
    if (p->destruct && p->data) {
      p->destruct(p->data);
      p->data = NULL;
    }
    DELETE(p);
  }
}

void *NS_(BatchDftFactory_F_data)(struct NS_(BatchDftFactory_F) *p) {
  return p->data;
}

struct NS(RealToComplexFunction) *NS_(BatchDftFactory_F_create)(
    struct NS_(BatchDftFactory_F) *p, struct NS(DataFactory) *data,
    Long dft_size, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride_0, Long y_stride_1, bool inplace, int num_threads) {
  assert(p);
  return p->create(p, data, dft_size, batch_size, x_stride_0, x_stride_1,
                   y_stride_0, y_stride_1, inplace, num_threads);
}

struct NS_(BatchDftFactory_B) {
  NS_(BatchDftFactory_B_fn) create;
  jomfftDestruct_fn destruct;
  void *data;
};

struct NS_(BatchDftFactory_B) *NS_(BatchDftFactory_B_new)(
    NS_(BatchDftFactory_B_fn) create, jomfftDestruct_fn destruct, void *data) {
  struct NS_(BatchDftFactory_B) *out = NEW(struct NS_(BatchDftFactory_B));
  *out = (struct NS_(BatchDftFactory_B)){
      .create = create, .destruct = destruct, .data = data};
  return out;
}

void NS_(BatchDftFactory_B_delete)(struct NS_(BatchDftFactory_B) *p) {
  if (p) {
    if (p->destruct && p->data) {
      p->destruct(p->data);
      p->data = NULL;
    }
    DELETE(p);
  }
}

void *NS_(BatchDftFactory_B_data)(struct NS_(BatchDftFactory_B) *p) {
  return p->data;
}

struct NS(ComplexToRealFunction) *NS_(BatchDftFactory_B_create)(
    struct NS_(BatchDftFactory_B) *p, struct NS(DataFactory) *data,
    Long dft_size, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride_0, Long y_stride_1, bool inplace, int num_threads) {
  assert(p);
  return p->create(p, data, dft_size, batch_size, x_stride_0, x_stride_1,
                   y_stride_0, y_stride_1, inplace, num_threads);
}

static void InnerBatchDft_FTest_(struct NS_(BatchDftFactory_F) *factory,
                                 Long dft_size, Long batch_size,
                                 int num_threads) {
  NS(Test_ClearCapture)();
  CAPTURE_INT(num_threads);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  struct NS(RealToComplexFunction) *func = NS_(BatchDftFactory_F_create)(
      factory, data, dft_size, batch_size, batch_size, 1, batch_size, 1, false,
      num_threads);
  NS_(InnerBatchDft_FTest)(func, dft_size, batch_size);
  NS(RealToComplexFunction_delete)(func);
  NS(DataFactory_delete)(data);
}

static void OuterBatchDft_FTest_(struct NS_(BatchDftFactory_F) *factory,
                                 Long dft_size, Long batch_size, Long x_stride,
                                 Long y_stride, bool inplace, int num_threads) {
  NS(Test_ClearCapture)();
  CAPTURE_INT(num_threads);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  const Long y_size = CE_SIZE(dft_size);
  const Long x_stride_1 = inplace ? 2 * y_size * x_stride : dft_size * x_stride;
  const Long y_stride_1 = y_size * y_stride;
  struct NS(RealToComplexFunction) *func = NS_(BatchDftFactory_F_create)(
      factory, data, dft_size, batch_size, x_stride, x_stride_1, y_stride,
      y_stride_1, inplace, num_threads);
  NS_(OuterBatchDft_FTest)
  (func, dft_size, batch_size, x_stride, y_stride, inplace);
  NS(RealToComplexFunction_delete)(func);
  NS(DataFactory_delete)(data);
}

static void TransposeDft_0_FTest_(struct NS_(BatchDftFactory_F) *factory,
                                  Long dft_size, Long batch_size,
                                  int num_threads) {
  NS(Test_ClearCapture)();
  CAPTURE_INT(num_threads);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  const Long y_size = CE_SIZE(dft_size);
  struct NS(RealToComplexFunction) *func = NS_(BatchDftFactory_F_create)(
      factory, data, dft_size, batch_size, batch_size, 1, 1, y_size, false,
      num_threads);
  NS_(TransposeDft_0_FTest)(func, dft_size, batch_size);
  NS(RealToComplexFunction_delete)(func);
  NS(DataFactory_delete)(data);
}

static void TransposeDft_1_FTest_(struct NS_(BatchDftFactory_F) *factory,
                                  Long dft_size, Long batch_size,
                                  int num_threads) {
  NS(Test_ClearCapture)();
  CAPTURE_INT(num_threads);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  struct NS(RealToComplexFunction) *func = NS_(BatchDftFactory_F_create)(
      factory, data, dft_size, batch_size, 1, dft_size, batch_size, 1, false,
      num_threads);
  NS_(TransposeDft_1_FTest)(func, dft_size, batch_size);
  NS(RealToComplexFunction_delete)(func);
  NS(DataFactory_delete)(data);
}

static void BatchDft_FTest_(struct NS_(BatchDftFactory_F) *factory,
                            Long dft_size, int num_threads) {
  InnerBatchDft_FTest_(factory, dft_size, 8, num_threads);
  InnerBatchDft_FTest_(factory, dft_size, 5, num_threads);
  OuterBatchDft_FTest_(factory, dft_size, 3, 1, 1, false, num_threads);
  OuterBatchDft_FTest_(factory, dft_size, 16, 1, 1, false, num_threads);
  OuterBatchDft_FTest_(factory, dft_size, 8, 2, 1, false, num_threads);
  OuterBatchDft_FTest_(factory, dft_size, 2, 1, 2, false, num_threads);
  OuterBatchDft_FTest_(factory, dft_size, 2, 2, 2, false, num_threads);
  OuterBatchDft_FTest_(factory, dft_size, 15, 1, 1, true, num_threads);
  OuterBatchDft_FTest_(factory, dft_size, 3, 1, 1, true, num_threads);
  TransposeDft_0_FTest_(factory, dft_size, 3, num_threads);
  TransposeDft_0_FTest_(factory, dft_size, 8, num_threads);
  TransposeDft_1_FTest_(factory, dft_size, 3, num_threads);
  TransposeDft_1_FTest_(factory, dft_size, 8, num_threads);
}

void NS_(BatchDft_FTest_)(struct NS_(BatchDftFactory_F) *factory,
                          Long dft_size) {
  BatchDft_FTest_(factory, dft_size, 1);
#if USE_OPENMP
  BatchDft_FTest_(factory, dft_size, 2);
  BatchDft_FTest_(factory, dft_size, omp_get_max_threads());
#endif
}

static void InnerBatchDft_BTest_(struct NS_(BatchDftFactory_B) *factory,
                                 Long dft_size, Long batch_size,
                                 int num_threads) {
  NS(Test_ClearCapture)();
  CAPTURE_INT(num_threads);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  struct NS(ComplexToRealFunction) *func = NS_(BatchDftFactory_B_create)(
      factory, data, dft_size, batch_size, batch_size, 1, batch_size, 1, false,
      num_threads);
  NS_(InnerBatchDft_BTest)(func, dft_size, batch_size);
  NS(ComplexToRealFunction_delete)(func);
  NS(DataFactory_delete)(data);
}

static void OuterBatchDft_BTest_(struct NS_(BatchDftFactory_B) *factory,
                                 Long dft_size, Long batch_size, Long x_stride,
                                 Long y_stride, bool inplace, int num_threads) {
  NS(Test_ClearCapture)();
  CAPTURE_INT(num_threads);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  const Long x_size = CE_SIZE(dft_size);
  const Long x_stride_1 = x_size * x_stride;
  const Long y_stride_1 = inplace ? 2 * x_size * y_stride : dft_size * y_stride;
  struct NS(ComplexToRealFunction) *func = NS_(BatchDftFactory_B_create)(
      factory, data, dft_size, batch_size, x_stride, x_stride_1, y_stride,
      y_stride_1, inplace, num_threads);
  NS_(OuterBatchDft_BTest)
  (func, dft_size, batch_size, x_stride, y_stride, inplace);
  NS(ComplexToRealFunction_delete)(func);
  NS(DataFactory_delete)(data);
}

static void TransposeDft_0_BTest_(struct NS_(BatchDftFactory_B) *factory,
                                  Long dft_size, Long batch_size,
                                  int num_threads) {
  NS(Test_ClearCapture)();
  CAPTURE_INT(num_threads);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  struct NS(ComplexToRealFunction) *func = NS_(BatchDftFactory_B_create)(
      factory, data, dft_size, batch_size, batch_size, 1, 1, dft_size, false,
      num_threads);
  NS_(TransposeDft_0_BTest)(func, dft_size, batch_size);
  NS(ComplexToRealFunction_delete)(func);
  NS(DataFactory_delete)(data);
}

static void TransposeDft_1_BTest_(struct NS_(BatchDftFactory_B) *factory,
                                  Long dft_size, Long batch_size,
                                  int num_threads) {
  NS(Test_ClearCapture)();
  CAPTURE_INT(num_threads);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  const Long x_size = CE_SIZE(dft_size);
  struct NS(ComplexToRealFunction) *func =
      NS_(BatchDftFactory_B_create)(factory, data, dft_size, batch_size, 1,
                                    x_size, batch_size, 1, false, num_threads);
  NS_(TransposeDft_1_BTest)(func, dft_size, batch_size);
  NS(ComplexToRealFunction_delete)(func);
  NS(DataFactory_delete)(data);
}

static void BatchDft_BTest_(struct NS_(BatchDftFactory_B) *factory,
                            Long dft_size, int num_threads) {
  InnerBatchDft_BTest_(factory, dft_size, 8, num_threads);
  InnerBatchDft_BTest_(factory, dft_size, 5, num_threads);
  OuterBatchDft_BTest_(factory, dft_size, 3, 1, 1, false, num_threads);
  OuterBatchDft_BTest_(factory, dft_size, 16, 1, 1, false, num_threads);
  OuterBatchDft_BTest_(factory, dft_size, 8, 2, 1, false, num_threads);
  OuterBatchDft_BTest_(factory, dft_size, 2, 1, 2, false, num_threads);
  OuterBatchDft_BTest_(factory, dft_size, 2, 2, 2, false, num_threads);
  OuterBatchDft_BTest_(factory, dft_size, 15, 1, 1, true, num_threads);
  OuterBatchDft_BTest_(factory, dft_size, 3, 1, 1, true, num_threads);
  TransposeDft_0_BTest_(factory, dft_size, 3, num_threads);
  TransposeDft_0_BTest_(factory, dft_size, 8, num_threads);
  TransposeDft_1_BTest_(factory, dft_size, 3, num_threads);
  TransposeDft_1_BTest_(factory, dft_size, 8, num_threads);
}

void NS_(BatchDft_BTest_)(struct NS_(BatchDftFactory_B) *factory,
                          Long dft_size) {
  BatchDft_BTest_(factory, dft_size, 1);
#if USE_OPENMP
  BatchDft_BTest_(factory, dft_size, 2);
  BatchDft_BTest_(factory, dft_size, omp_get_max_threads());
#endif
}
