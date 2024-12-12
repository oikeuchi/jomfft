#include "BatchDftTest_.h"
#include "InnerBatchDftTest.h"
#include "OuterBatchDftTest.h"
#include "TransposeDftTest.h"

#define NS_(n) NS(CDft_##n)

struct NS_(BatchDftFactory) {
  NS_(BatchDftFactory_fn) create;
  jomfftDestruct_fn destruct;
  void *data;
};

struct NS_(BatchDftFactory) *NS_(BatchDftFactory_new)(
    NS_(BatchDftFactory_fn) create, jomfftDestruct_fn destruct, void *data) {
  struct NS_(BatchDftFactory) *out = NEW(struct NS_(BatchDftFactory));
  *out = (struct NS_(BatchDftFactory)){
      .create = create, .destruct = destruct, .data = data};
  return out;
}

void NS_(BatchDftFactory_delete)(struct NS_(BatchDftFactory) *p) {
  if (p) {
    if (p->destruct && p->data) {
      p->destruct(p->data);
      p->data = NULL;
    }
    DELETE(p);
  }
}

void *NS_(BatchDftFactory_data)(struct NS_(BatchDftFactory) *p) {
  return p->data;
}

struct NS(ComplexFunction) *NS_(BatchDftFactory_create)(
    struct NS_(BatchDftFactory) *p, struct NS(DataFactory) *data, bool backward,
    Long dft_size, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride_0, Long y_stride_1, bool inplace, int num_threads) {
  assert(p);
  return p->create(p, data, backward, dft_size, batch_size, x_stride_0,
                   x_stride_1, y_stride_0, y_stride_1, inplace, num_threads);
}

static void InnerBatchDftTest_(struct NS_(BatchDftFactory) *factory,
                               bool backward, Long dft_size, Long batch_size,
                               bool inplace, int num_threads) {
  NS(Test_ClearCapture)();
  CAPTURE_INT(num_threads);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  struct NS(ComplexFunction) *func = NS_(BatchDftFactory_create)(
      factory, data, backward, dft_size, batch_size, batch_size, 1, batch_size,
      1, inplace, num_threads);
  NS_(InnerBatchDftTest)(func, backward, dft_size, batch_size, inplace);
  NS(ComplexFunction_delete)(func);
  NS(DataFactory_delete)(data);
}

static void OuterBatchDftTest_(struct NS_(BatchDftFactory) *factory,
                               bool backward, Long dft_size, Long batch_size,
                               Long x_stride, Long y_stride, bool inplace,
                               int num_threads) {
  NS(Test_ClearCapture)();
  CAPTURE_INT(num_threads);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  const Long x_stride_1 = dft_size * x_stride;
  const Long y_stride_1 = dft_size * y_stride;
  struct NS(ComplexFunction) *func = NS_(BatchDftFactory_create)(
      factory, data, backward, dft_size, batch_size, x_stride, x_stride_1,
      y_stride, y_stride_1, inplace, num_threads);
  NS_(OuterBatchDftTest)
  (func, backward, dft_size, batch_size, x_stride, y_stride, inplace);
  NS(ComplexFunction_delete)(func);
  NS(DataFactory_delete)(data);
}

static void TransposeDft_0Test_(struct NS_(BatchDftFactory) *factory,
                                bool backward, Long dft_size, Long batch_size,
                                int num_threads) {
  NS(Test_ClearCapture)();
  CAPTURE_INT(num_threads);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  struct NS(ComplexFunction) *func = NS_(BatchDftFactory_create)(
      factory, data, backward, dft_size, batch_size, batch_size, 1, 1, dft_size,
      false, num_threads);
  NS_(TransposeDft_0Test)(func, backward, dft_size, batch_size);
  NS(ComplexFunction_delete)(func);
  NS(DataFactory_delete)(data);
}

static void TransposeDft_1Test_(struct NS_(BatchDftFactory) *factory,
                                bool backward, Long dft_size, Long batch_size,
                                int num_threads) {
  NS(Test_ClearCapture)();
  CAPTURE_INT(num_threads);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  struct NS(ComplexFunction) *func = NS_(BatchDftFactory_create)(
      factory, data, backward, dft_size, batch_size, 1, dft_size, batch_size, 1,
      false, num_threads);
  NS_(TransposeDft_1Test)(func, backward, dft_size, batch_size);
  NS(ComplexFunction_delete)(func);
  NS(DataFactory_delete)(data);
}

static void BatchDftTest_(struct NS_(BatchDftFactory) *factory, Long dft_size,
                          int num_threads) {
  InnerBatchDftTest_(factory, false, dft_size, 3, false, num_threads);
  InnerBatchDftTest_(factory, true, dft_size, 4, false, num_threads);
  InnerBatchDftTest_(factory, false, dft_size, 8, true, num_threads);
  InnerBatchDftTest_(factory, true, dft_size, 3, true, num_threads);
  OuterBatchDftTest_(factory, false, dft_size, 7, 1, 1, false, num_threads);
  OuterBatchDftTest_(factory, true, dft_size, 16, 1, 1, false, num_threads);
  OuterBatchDftTest_(factory, false, dft_size, 3, 2, 1, false, num_threads);
  OuterBatchDftTest_(factory, true, dft_size, 6, 1, 2, false, num_threads);
  OuterBatchDftTest_(factory, true, dft_size, 1, 2, 2, false, num_threads);
  OuterBatchDftTest_(factory, false, dft_size, 9, 1, 1, true, num_threads);
  OuterBatchDftTest_(factory, true, dft_size, 3, 1, 1, true, num_threads);
  OuterBatchDftTest_(factory, false, dft_size, 5, 2, 2, true, num_threads);
  OuterBatchDftTest_(factory, true, dft_size, 3, 2, 2, true, num_threads);
  TransposeDft_0Test_(factory, false, dft_size, 7, num_threads);
  TransposeDft_0Test_(factory, true, dft_size, 4, num_threads);
  TransposeDft_1Test_(factory, false, dft_size, 3, num_threads);
  TransposeDft_1Test_(factory, true, dft_size, 4, num_threads);
}

void NS_(BatchDftTest_)(struct NS_(BatchDftFactory) *factory, Long dft_size) {
  BatchDftTest_(factory, dft_size, 1);
#if USE_OPENMP
  BatchDftTest_(factory, dft_size, 2);
  BatchDftTest_(factory, dft_size, omp_get_max_threads());
#endif
}
