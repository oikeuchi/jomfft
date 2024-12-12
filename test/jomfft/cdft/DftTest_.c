#include "DftTest_.h"

#include <jomfft/util/random/ComplexTensor.h>

#include "naive/Dft.h"

#define NS_(n) NS(CDft_##n)

void NS_(DftTest__)(struct NS(ComplexFunction) *func, bool backward,
                    Long dft_size, Long x_stride, Long y_stride, bool inplace) {
  assert(func);
  assert(x_stride >= 1);
  assert(y_stride >= 1);
  CAPTURE_BOOL(backward);
  CAPTURE_LONG(dft_size);
  CAPTURE_LONG(x_stride);
  CAPTURE_LONG(y_stride);
  CAPTURE_BOOL(inplace);
  struct NS(ComplexTensor) *x =
      NS(Random_ComplexTensor_new2)(dft_size, x_stride);
  struct NS(ComplexTensor) *x0 = NS(ComplexTensor_copy)(x);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_new2)(dft_size, y_stride);
  if (inplace) {
    assert(x_stride == y_stride);
    {
      Long sizes[] = {dft_size, 1};
      struct NS(TensorShape) *shape = NS(TensorShape_new)(2, sizes);
      NS(ComplexTensor_CopyAs)(shape, x0, y0);
      NS(TensorShape_delete)(shape);
    }
    NS(ComplexFunction_evaluate)(func, y0->data, y0->data);
  } else {
    NS(ComplexFunction_evaluate)(func, x0->data, y0->data);
  }
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(y0);
  NS(CDftNaive_Dft)(backward, dft_size, x->data, x_stride, y1->data, y_stride);
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(x0);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
}

struct NS_(DftFactory) {
  NS_(DftFactory_fn) create;
  jomfftDestruct_fn destruct;
  void *data;
};

struct NS_(DftFactory) *NS_(DftFactory_new)(NS_(DftFactory_fn) create,
                                            jomfftDestruct_fn destruct,
                                            void *data) {
  struct NS_(DftFactory) *out = NEW(struct NS_(DftFactory));
  *out = (struct NS_(DftFactory)){
      .create = create, .destruct = destruct, .data = data};
  return out;
}

void NS_(DftFactory_delete)(struct NS_(DftFactory) *p) {
  if (p) {
    if (p->destruct && p->data) {
      p->destruct(p->data);
      p->data = NULL;
    }
    DELETE(p);
  }
}

void *NS_(DftFactory_data)(struct NS_(DftFactory) *p) { return p->data; }

struct NS(ComplexFunction) *NS_(DftFactory_create)(struct NS_(DftFactory) *p,
                                                   struct NS(DataFactory) *data,
                                                   bool backward, Long dft_size,
                                                   Long x_stride, Long y_stride,
                                                   bool inplace,
                                                   int num_threads) {
  assert(p);
  return p->create(p, data, backward, dft_size, x_stride, y_stride, inplace,
                   num_threads);
}

static void DftTest__(struct NS_(DftFactory) *factory, bool backward,
                      Long dft_size, Long x_stride, Long y_stride, bool inplace,
                      int num_threads) {
  CAPTURE_INT(num_threads);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  struct NS(ComplexFunction) *func =
      NS_(DftFactory_create)(factory, data, backward, dft_size, x_stride,
                             y_stride, inplace, num_threads);
  NS_(DftTest__)(func, backward, dft_size, x_stride, y_stride, inplace);
  NS(ComplexFunction_delete)(func);
  NS(DataFactory_delete)(data);
}

static void DftTest_(struct NS_(DftFactory) *factory, Long dft_size,
                     int num_threads) {
  DftTest__(factory, false, dft_size, 1, 1, false, num_threads);
  DftTest__(factory, true, dft_size, 1, 1, false, num_threads);
  DftTest__(factory, false, dft_size, 2, 1, false, num_threads);
  DftTest__(factory, true, dft_size, 1, 2, false, num_threads);
  DftTest__(factory, true, dft_size, 2, 2, false, num_threads);
  DftTest__(factory, false, dft_size, 1, 1, true, num_threads);
  DftTest__(factory, true, dft_size, 1, 1, true, num_threads);
  DftTest__(factory, false, dft_size, 2, 2, true, num_threads);
  DftTest__(factory, true, dft_size, 2, 2, true, num_threads);
}

void NS_(DftTest_)(struct NS_(DftFactory) *factory, Long dft_size) {
  DftTest_(factory, dft_size, 1);
#if USE_OPENMP
  DftTest_(factory, dft_size, 2);
  DftTest_(factory, dft_size, omp_get_max_threads());
#endif
}
