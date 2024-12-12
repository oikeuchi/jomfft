#include "EvenSizeDft.h"

#include "../../cdft/bluestein/Dft.h"
#include "../../model/ComplexCopy.h"
#include "../../model/ComplexToRealFunctionBuffered.h"
#include "../../model/ComplexToRealFunctionPairW.h"
#include "../../model/ComplexToRealFunctionTripleW.h"
#include "../../model/RealCopy.h"
#include "../../model/RealToComplexFunctionBuffered.h"
#include "../../model/RealToComplexFunctionPairW.h"
#include "../../model/RealToComplexFunctionTripleW.h"
#include "../core/EvenSizeTwiddleDft.h"

#define NS_(n) NS(RDftBluestein_##n)

struct NS_(EvenSizeDftData) {
  struct NS(ComplexFunctionW) *dft;
  struct NS(ComplexFunction) *twiddle_dft;
};

static struct NS_(EvenSizeDftData) *NS_(EvenSizeDftData_new)(
    struct NS(ComplexFunctionW) *dft, struct NS(ComplexFunction) *twiddle_dft) {
  struct NS_(EvenSizeDftData) *out = NEW(struct NS_(EvenSizeDftData));
  *out = (struct NS_(EvenSizeDftData)){.dft = dft, .twiddle_dft = twiddle_dft};
  return out;
}

static void NS_(EvenSizeDftData_destruct)(void *p) {
  if (p) {
    struct NS_(EvenSizeDftData) *o = (struct NS_(EvenSizeDftData) *)p;
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

static void NS_(EvenSizeDft_F_evaluate)(struct NS(RealToComplexFunctionW) *p,
                                        Real *x, Complex *y, void *work) {
  struct NS_(EvenSizeDftData) *data =
      (struct NS_(EvenSizeDftData) *)NS(RealToComplexFunctionW_data)(p);
  assert(data);
  Complex *b = (Complex *)work;
  NS(ComplexFunctionW_evaluate)(data->dft, (Complex *)x, b, b);
  NS(ComplexFunction_evaluate)(data->twiddle_dft, b, y);
}

static void NS_(EvenSizeDft_B_evaluate)(struct NS(ComplexToRealFunctionW) *p,
                                        Complex *x, Real *y, void *work) {
  struct NS_(EvenSizeDftData) *data =
      (struct NS_(EvenSizeDftData) *)NS(ComplexToRealFunctionW_data)(p);
  assert(data);
  Complex *b = (Complex *)work;
  NS(ComplexFunction_evaluate)(data->twiddle_dft, x, b);
  NS(ComplexFunctionW_evaluate)(data->dft, b, (Complex *)y, b);
}

static Long NS_(EvenSizeDft_F_size)(struct NS(RealToComplexFunctionW) *p) {
  struct NS_(EvenSizeDftData) *data =
      (struct NS_(EvenSizeDftData) *)NS(RealToComplexFunctionW_data)(p);
  assert(data);
  return NS(ComplexFunctionW_size)(data->dft);
}

static Long NS_(EvenSizeDft_B_size)(struct NS(ComplexToRealFunctionW) *p) {
  struct NS_(EvenSizeDftData) *data =
      (struct NS_(EvenSizeDftData) *)NS(ComplexToRealFunctionW_data)(p);
  assert(data);
  return NS(ComplexFunctionW_size)(data->dft);
}

static struct NS(RealToComplexFunctionW) *NS_(EvenSizeDft_F_newW_)(
    struct NS(DataFactory) *data, Long dft_size, int num_threads) {
  return NS(RealToComplexFunctionW_new)(
      NS_(EvenSizeDft_F_evaluate), NS_(EvenSizeDft_F_size),
      NS_(EvenSizeDftData_destruct),
      NS_(EvenSizeDftData_new)(
          NS(CDftBluestein_Dft_newW)(data, false, dft_size, 1, 1, num_threads),
          NS(RDftCore_EvenSizeTwiddleDft_F_new_)(data, dft_size, num_threads)));
}

struct NS(RealToComplexFunctionW) *NS_(EvenSizeDft_F_newW)(
    struct NS(DataFactory) *data, Long dft_size, Long x_stride, Long y_stride,
    int num_threads) {
  const Long x_size = 2 * dft_size;
  const Long y_size = CE_SIZE(2 * dft_size);
  if (x_stride == 1 && y_stride == 1) {
    return NS_(EvenSizeDft_F_newW_)(data, dft_size, num_threads);
  } else if (y_stride == 1) {
    return NS(RealToComplexFunctionPairWB_0_new)(
        NS(Real_Copy_new_)(x_size, x_stride, 1, num_threads),
        NS_(EvenSizeDft_F_newW_)(data, dft_size, num_threads));
  } else if (x_stride == 1) {
    return NS(RealToComplexFunctionPairWB_1_new)(
        NS_(EvenSizeDft_F_newW_)(data, dft_size, num_threads),
        NS(Complex_Copy_new_)(y_size, 1, y_stride, num_threads));
  } else {
    return NS(RealToComplexFunctionTripleWB_1_new)(
        NS(Real_Copy_new_)(x_size, x_stride, 1, num_threads),
        NS_(EvenSizeDft_F_newW_)(data, dft_size, num_threads),
        NS(Complex_Copy_new_)(y_size, 1, y_stride, num_threads));
  }
}

struct NS(RealToComplexFunction) *NS_(EvenSizeDft_F_new)(
    struct NS(DataFactory) *data, Long dft_size, Long x_stride, Long y_stride,
    int num_threads) {
  return NS(RealToComplexFunctionBuffered_new)(
      NS_(EvenSizeDft_F_newW)(data, dft_size, x_stride, y_stride, num_threads));
}

static struct NS(ComplexToRealFunctionW) *NS_(EvenSizeDft_B_newW_)(
    struct NS(DataFactory) *data, Long dft_size, int num_threads) {
  return NS(ComplexToRealFunctionW_new)(
      NS_(EvenSizeDft_B_evaluate), NS_(EvenSizeDft_B_size),
      NS_(EvenSizeDftData_destruct),
      NS_(EvenSizeDftData_new)(
          NS(CDftBluestein_Dft_newW)(data, true, dft_size, 1, 1, num_threads),
          NS(RDftCore_EvenSizeTwiddleDft_B_new_)(data, dft_size, num_threads)));
}

struct NS(ComplexToRealFunctionW) *NS_(EvenSizeDft_B_newW)(
    struct NS(DataFactory) *data, Long dft_size, Long x_stride, Long y_stride,
    int num_threads) {
  const Long x_size = CE_SIZE(2 * dft_size);
  const Long y_size = 2 * dft_size;
  if (x_stride == 1 && y_stride == 1) {
    return NS_(EvenSizeDft_B_newW_)(data, dft_size, num_threads);
  } else if (x_stride == 1) {
    return NS(ComplexToRealFunctionPairWB_1_new)(
        NS_(EvenSizeDft_B_newW_)(data, dft_size, num_threads),
        NS(Real_Copy_new_)(y_size, 1, y_stride, num_threads));
  } else if (y_stride == 1) {
    return NS(ComplexToRealFunctionPairWB_0_new)(
        NS(Complex_Copy_new_)(x_size, x_stride, 1, num_threads),
        NS_(EvenSizeDft_B_newW_)(data, dft_size, num_threads));
  } else {
    return NS(ComplexToRealFunctionTripleWB_1_new)(
        NS(Complex_Copy_new_)(x_size, x_stride, 1, num_threads),
        NS_(EvenSizeDft_B_newW_)(data, dft_size, num_threads),
        NS(Real_Copy_new_)(y_size, 1, y_stride, num_threads));
  }
}

struct NS(ComplexToRealFunction) *NS_(EvenSizeDft_B_new)(
    struct NS(DataFactory) *data, Long dft_size, Long x_stride, Long y_stride,
    int num_threads) {
  return NS(ComplexToRealFunctionBuffered_new)(
      NS_(EvenSizeDft_B_newW)(data, dft_size, x_stride, y_stride, num_threads));
}
