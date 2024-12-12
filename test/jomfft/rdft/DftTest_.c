#include "DftTest_.h"

#include <jomfft/util/random/ConjugateEvenTensor.h>
#include <jomfft/util/random/RealTensor.h>

#include "naive/Dft.h"

#define NS_(n) NS(RDft_##n)

void NS_(Dft_FTest__)(struct NS(RealToComplexFunction) *func, Long dft_size,
                      Long x_stride, Long y_stride, bool inplace) {
  assert(func);
  assert(x_stride >= 1);
  assert(y_stride >= 1);
  CAPTURE_LONG(dft_size);
  CAPTURE_LONG(x_stride);
  CAPTURE_LONG(y_stride);
  CAPTURE_BOOL(inplace);
  const Long y_size = CE_SIZE(dft_size);
  if (inplace && x_stride == 1 && y_stride == 1) {
    struct NS(RealTensor) *x = NS(Random_RealTensor_new1)(dft_size);
    struct NS(ComplexTensor) *y0 = NS(ComplexTensor_new1)(y_size);
    ARRAY_COPY(Real, dft_size, x->data, (Real *)y0->data);
    NS(RealToComplexFunction_evaluate)(func, (Real *)y0->data, y0->data);
    struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(y0);
    NS(RDftNaive_Dft_F)(dft_size, x->data, 1, y1->data, 1);
    ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
    NS(RealTensor_delete)(x);
    NS(ComplexTensor_delete)(y0);
    NS(ComplexTensor_delete)(y1);
  } else {
    struct NS(RealTensor) *x = NS(Random_RealTensor_new2)(dft_size, x_stride);
    struct NS(RealTensor) *x0 = NS(RealTensor_copy)(x);
    struct NS(ComplexTensor) *y0 = NS(ComplexTensor_new2)(y_size, y_stride);
    NS(RealToComplexFunction_evaluate)(func, x0->data, y0->data);
    struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(y0);
    NS(RDftNaive_Dft_F)(dft_size, x->data, x_stride, y1->data, y_stride);
    ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
    NS(RealTensor_delete)(x);
    NS(RealTensor_delete)(x0);
    NS(ComplexTensor_delete)(y0);
    NS(ComplexTensor_delete)(y1);
  }
}

void NS_(Dft_BTest__)(struct NS(ComplexToRealFunction) *func, Long dft_size,
                      Long x_stride, Long y_stride, bool inplace) {
  assert(func);
  assert(x_stride >= 1);
  assert(y_stride >= 1);
  CAPTURE_LONG(dft_size);
  CAPTURE_LONG(x_stride);
  CAPTURE_LONG(y_stride);
  CAPTURE_BOOL(inplace);
  const Long x_size = CE_SIZE(dft_size);
  const Long y_size = 2 * x_size;
  if (inplace && x_stride == 1 && y_stride == 1) {
    struct NS(ComplexTensor) *x = NS(Random_ConjugateEvenTensor_new1)(dft_size);
    struct NS(RealTensor) *y0 = NS(RealTensor_new1)(y_size);
    struct NS(RealTensor) *y1 = NS(RealTensor_zerosLike)(y0);
    ARRAY_COPY(Complex, x_size, x->data, (Complex *)y0->data);
    ARRAY_COPY(Complex, x_size, x->data, (Complex *)y1->data);
    NS(ComplexToRealFunction_evaluate)
    (func, (Complex *)y0->data, y0->data);
    NS(RDftNaive_Dft_B)(dft_size, x->data, 1, y1->data, 1);
    ASSERT_REALTENSOR_AEQ(y0, y1);
    NS(ComplexTensor_delete)(x);
    NS(RealTensor_delete)(y0);
    NS(RealTensor_delete)(y1);
  } else {
    const Long sizes[] = {dft_size, x_stride};
    struct NS(ComplexTensor) *x =
        NS(Random_ConjugateEvenTensor_new_)(2, sizes, 0);
    struct NS(ComplexTensor) *x0 = NS(ComplexTensor_copy)(x);
    struct NS(RealTensor) *y0 = NS(RealTensor_new2)(y_size, y_stride);
    NS(ComplexToRealFunction_evaluate)(func, x0->data, y0->data);
    struct NS(RealTensor) *y1 = NS(RealTensor_zerosLike)(y0);
    NS(RDftNaive_Dft_B)(dft_size, x->data, x_stride, y1->data, y_stride);
    ASSERT_REALTENSOR_AEQ(y0, y1);
    NS(ComplexTensor_delete)(x);
    NS(ComplexTensor_delete)(x0);
    NS(RealTensor_delete)(y0);
    NS(RealTensor_delete)(y1);
  }
}

struct NS_(DftFactory_F) {
  NS_(DftFactory_F_fn) create;
  jomfftDestruct_fn destruct;
  void *data;
};

struct NS_(DftFactory_F) *NS_(DftFactory_F_new)(NS_(DftFactory_F_fn) create,
                                                jomfftDestruct_fn destruct,
                                                void *data) {
  struct NS_(DftFactory_F) *out = NEW(struct NS_(DftFactory_F));
  *out = (struct NS_(DftFactory_F)){
      .create = create, .destruct = destruct, .data = data};
  return out;
}

void NS_(DftFactory_F_delete)(struct NS_(DftFactory_F) *p) {
  if (p) {
    if (p->destruct && p->data) {
      p->destruct(p->data);
      p->data = NULL;
    }
    DELETE(p);
  }
}

void *NS_(DftFactory_F_data)(struct NS_(DftFactory_F) *p) { return p->data; }

struct NS(RealToComplexFunction) *NS_(DftFactory_F_create)(
    struct NS_(DftFactory_F) *p, struct NS(DataFactory) *data, Long dft_size,
    Long x_stride, Long y_stride, bool inplace, int num_threads) {
  assert(p);
  return p->create(p, data, dft_size, x_stride, y_stride, inplace, num_threads);
}

struct NS_(DftFactory_B) {
  NS_(DftFactory_B_fn) create;
  jomfftDestruct_fn destruct;
  void *data;
};

struct NS_(DftFactory_B) *NS_(DftFactory_B_new)(NS_(DftFactory_B_fn) create,
                                                jomfftDestruct_fn destruct,
                                                void *data) {
  struct NS_(DftFactory_B) *out = NEW(struct NS_(DftFactory_B));
  *out = (struct NS_(DftFactory_B)){
      .create = create, .destruct = destruct, .data = data};
  return out;
}

void NS_(DftFactory_B_delete)(struct NS_(DftFactory_B) *p) {
  if (p) {
    if (p->destruct && p->data) {
      p->destruct(p->data);
      p->data = NULL;
    }
    DELETE(p);
  }
}

void *NS_(DftFactory_B_data)(struct NS_(DftFactory_B) *p) { return p->data; }

struct NS(ComplexToRealFunction) *NS_(DftFactory_B_create)(
    struct NS_(DftFactory_B) *p, struct NS(DataFactory) *data, Long dft_size,
    Long x_stride, Long y_stride, bool inplace, int num_threads) {
  assert(p);
  return p->create(p, data, dft_size, x_stride, y_stride, inplace, num_threads);
}

static void Dft_FTest__(struct NS_(DftFactory_F) *factory, Long dft_size,
                        Long x_stride, Long y_stride, bool inplace,
                        int num_threads) {
  CAPTURE_INT(num_threads);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  struct NS(RealToComplexFunction) *func = NS_(DftFactory_F_create)(
      factory, data, dft_size, x_stride, y_stride, inplace, num_threads);
  NS_(Dft_FTest__)(func, dft_size, x_stride, y_stride, inplace);
  NS(RealToComplexFunction_delete)(func);
  NS(DataFactory_delete)(data);
}

static void Dft_BTest__(struct NS_(DftFactory_B) *factory, Long dft_size,
                        Long x_stride, Long y_stride, bool inplace,
                        int num_threads) {
  CAPTURE_INT(num_threads);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  struct NS(ComplexToRealFunction) *func = NS_(DftFactory_B_create)(
      factory, data, dft_size, x_stride, y_stride, inplace, num_threads);
  NS_(Dft_BTest__)(func, dft_size, x_stride, y_stride, inplace);
  NS(ComplexToRealFunction_delete)(func);
  NS(DataFactory_delete)(data);
}

static void Dft_FTest_(struct NS_(DftFactory_F) *factory, Long dft_size,
                       int num_threads) {
  Dft_FTest__(factory, dft_size, 1, 1, false, num_threads);
  Dft_FTest__(factory, dft_size, 2, 1, false, num_threads);
  Dft_FTest__(factory, dft_size, 1, 2, false, num_threads);
  Dft_FTest__(factory, dft_size, 2, 2, false, num_threads);
  Dft_FTest__(factory, dft_size, 2, 2, true, num_threads);
  Dft_FTest__(factory, dft_size, 2, 2, true, num_threads);
}

void NS_(Dft_FTest_)(struct NS_(DftFactory_F) *factory, Long dft_size) {
  Dft_FTest_(factory, dft_size, 1);
#if USE_OPENMP
  Dft_FTest_(factory, dft_size, 2);
  Dft_FTest_(factory, dft_size, omp_get_max_threads());
#endif
}

static void Dft_BTest_(struct NS_(DftFactory_B) *factory, Long dft_size,
                       int num_threads) {
  Dft_BTest__(factory, dft_size, 1, 1, false, num_threads);
  Dft_BTest__(factory, dft_size, 2, 1, false, num_threads);
  Dft_BTest__(factory, dft_size, 1, 2, false, num_threads);
  Dft_BTest__(factory, dft_size, 2, 2, false, num_threads);
  Dft_BTest__(factory, dft_size, 2, 2, true, num_threads);
  Dft_BTest__(factory, dft_size, 2, 2, true, num_threads);
}

void NS_(Dft_BTest_)(struct NS_(DftFactory_B) *factory, Long dft_size) {
  Dft_BTest_(factory, dft_size, 1);
#if USE_OPENMP
  Dft_BTest_(factory, dft_size, 2);
  Dft_BTest_(factory, dft_size, omp_get_max_threads());
#endif
}
