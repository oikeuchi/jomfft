#include "EvenSizeDft.h"

#include "../model/ComplexFunctionLoop.h"
#include "core/EvenSizeTwiddleDft.h"

#define NS_(n) NS(RDft_##n)

struct NS_(EvenSizeDftData) {
  struct NS(ComplexFunction) *dft;
  struct NS(ComplexFunction) *twiddle_dft;
};

static struct NS_(EvenSizeDftData) *NS_(EvenSizeDftData_new)(
    struct NS(ComplexFunction) *dft, struct NS(ComplexFunction) *twiddle_dft) {
  struct NS_(EvenSizeDftData) *out = NEW(struct NS_(EvenSizeDftData));
  *out = (struct NS_(EvenSizeDftData)){.dft = dft, .twiddle_dft = twiddle_dft};
  return out;
}

static void NS_(EvenSizeDftData_destruct)(void *p) {
  if (p) {
    struct NS_(EvenSizeDftData) *o = (struct NS_(EvenSizeDftData) *)p;
    if (o->dft) {
      NS(ComplexFunction_delete)(o->dft);
      o->dft = NULL;
    }
    if (o->twiddle_dft) {
      NS(ComplexFunction_delete)(o->twiddle_dft);
      o->twiddle_dft = NULL;
    }
    DELETE(o);
  }
}

static void NS_(EvenSizeDft_F_evaluate)(struct NS(RealToComplexFunction) *p,
                                        Real *x, Complex *y) {
  struct NS_(EvenSizeDftData) *data =
      (struct NS_(EvenSizeDftData) *)NS(RealToComplexFunction_data)(p);
  assert(data);
  NS(ComplexFunction_evaluate)(data->dft, (Complex *)x, y);
  NS(ComplexFunction_evaluate)(data->twiddle_dft, y, y);
}

static void NS_(EvenSizeDft_B_evaluate)(struct NS(ComplexToRealFunction) *p,
                                        Complex *x, Real *y) {
  struct NS_(EvenSizeDftData) *data =
      (struct NS_(EvenSizeDftData) *)NS(ComplexToRealFunction_data)(p);
  assert(data);
  NS(ComplexFunction_evaluate)(data->twiddle_dft, x, x);
  NS(ComplexFunction_evaluate)(data->dft, x, (Complex *)y);
}

struct NS_(EvenSizeDftWData) {
  struct NS(ComplexFunctionW) *dft;
  struct NS(ComplexFunction) *twiddle_dft;
};

static struct NS_(EvenSizeDftWData) *NS_(EvenSizeDftWData_new)(
    struct NS(ComplexFunctionW) *dft, struct NS(ComplexFunction) *twiddle_dft) {
  struct NS_(EvenSizeDftWData) *out = NEW(struct NS_(EvenSizeDftWData));
  *out = (struct NS_(EvenSizeDftWData)){.dft = dft, .twiddle_dft = twiddle_dft};
  return out;
}

static void NS_(EvenSizeDftWData_destruct)(void *p) {
  if (p) {
    struct NS_(EvenSizeDftWData) *o = (struct NS_(EvenSizeDftWData) *)p;
    if (o->dft) {
      NS(ComplexFunctionW_delete)(o->dft);
      o->dft = NULL;
    }
    if (o->twiddle_dft) {
      NS(ComplexFunction_delete)(o->twiddle_dft);
      o->twiddle_dft = NULL;
    }
    DELETE(o);
  }
}

static void NS_(EvenSizeDftW_F_evaluate)(struct NS(RealToComplexFunctionW) *p,
                                         Real *x, Complex *y, void *work) {
  struct NS_(EvenSizeDftWData) *data =
      (struct NS_(EvenSizeDftWData) *)NS(RealToComplexFunctionW_data)(p);
  assert(data);
  NS(ComplexFunctionW_evaluate)(data->dft, (Complex *)x, y, work);
  NS(ComplexFunction_evaluate)(data->twiddle_dft, y, y);
}

static Long NS_(EvenSizeDftW_F_size)(struct NS(RealToComplexFunctionW) *p) {
  struct NS_(EvenSizeDftWData) *data =
      (struct NS_(EvenSizeDftWData) *)NS(RealToComplexFunctionW_data)(p);
  assert(data);
  return NS(ComplexFunctionW_size)(data->dft);
}

static void NS_(EvenSizeDftW_B_evaluate)(struct NS(ComplexToRealFunctionW) *p,
                                         Complex *x, Real *y, void *work) {
  struct NS_(EvenSizeDftWData) *data =
      (struct NS_(EvenSizeDftWData) *)NS(ComplexToRealFunctionW_data)(p);
  assert(data);
  NS(ComplexFunction_evaluate)(data->twiddle_dft, x, x);
  NS(ComplexFunctionW_evaluate)(data->dft, x, (Complex *)y, work);
}

static Long NS_(EvenSizeDftW_B_size)(struct NS(ComplexToRealFunctionW) *p) {
  struct NS_(EvenSizeDftWData) *data =
      (struct NS_(EvenSizeDftWData) *)NS(ComplexToRealFunctionW_data)(p);
  assert(data);
  return NS(ComplexFunctionW_size)(data->dft);
}

struct NS(RealToComplexFunction) *NS_(EvenSizeDft_F_new)(
    struct NS(DataFactory) *data, struct NS(ComplexFunction) *dft,
    Long dft_size, int num_threads) {
  return NS(RealToComplexFunction_new)(
      NS_(EvenSizeDft_F_evaluate), NS_(EvenSizeDftData_destruct),
      NS_(EvenSizeDftData_new)(dft, NS(RDftCore_EvenSizeTwiddleDft_F_new_)(
                                        data, dft_size, num_threads)));
}

struct NS(ComplexToRealFunction) *NS_(EvenSizeDft_B_new)(
    struct NS(DataFactory) *data, struct NS(ComplexFunction) *dft,
    Long dft_size, int num_threads) {
  return NS(ComplexToRealFunction_new)(
      NS_(EvenSizeDft_B_evaluate), NS_(EvenSizeDftData_destruct),
      NS_(EvenSizeDftData_new)(dft, NS(RDftCore_EvenSizeTwiddleDft_B_new_)(
                                        data, dft_size, num_threads)));
}

struct NS(RealToComplexFunctionW) *NS_(EvenSizeDftW_F_new)(
    struct NS(DataFactory) *data, struct NS(ComplexFunctionW) *dft,
    Long dft_size, int num_threads) {
  return NS(RealToComplexFunctionW_new)(
      NS_(EvenSizeDftW_F_evaluate), NS_(EvenSizeDftW_F_size),
      NS_(EvenSizeDftWData_destruct),
      NS_(EvenSizeDftWData_new)(dft, NS(RDftCore_EvenSizeTwiddleDft_F_new_)(
                                         data, dft_size, num_threads)));
}

struct NS(ComplexToRealFunctionW) *NS_(EvenSizeDftW_B_new)(
    struct NS(DataFactory) *data, struct NS(ComplexFunctionW) *dft,
    Long dft_size, int num_threads) {
  return NS(ComplexToRealFunctionW_new)(
      NS_(EvenSizeDftW_B_evaluate), NS_(EvenSizeDftW_B_size),
      NS_(EvenSizeDftWData_destruct),
      NS_(EvenSizeDftWData_new)(dft, NS(RDftCore_EvenSizeTwiddleDft_B_new_)(
                                         data, dft_size, num_threads)));
}

struct NS(RealToComplexFunction) *NS_(EvenSizeOuterBatchDft_F_new)(
    struct NS(DataFactory) *data, struct NS(ComplexFunction) *dft,
    Long dft_size, Long batch_size, Long stride) {
  return NS(RealToComplexFunction_new)(
      NS_(EvenSizeDft_F_evaluate), NS_(EvenSizeDftData_destruct),
      NS_(EvenSizeDftData_new)(
          dft, NS(ComplexFunctionLoop_new)(
                   NS(RDftCore_EvenSizeTwiddleDft_F_new)(data, dft_size),
                   batch_size, stride, stride)));
}

struct NS(ComplexToRealFunction) *NS_(EvenSizeOuterBatchDft_B_new)(
    struct NS(DataFactory) *data, struct NS(ComplexFunction) *dft,
    Long dft_size, Long batch_size, Long stride) {
  return NS(ComplexToRealFunction_new)(
      NS_(EvenSizeDft_B_evaluate), NS_(EvenSizeDftData_destruct),
      NS_(EvenSizeDftData_new)(
          dft, NS(ComplexFunctionLoop_new)(
                   NS(RDftCore_EvenSizeTwiddleDft_B_new)(data, dft_size),
                   batch_size, stride, stride)));
}
