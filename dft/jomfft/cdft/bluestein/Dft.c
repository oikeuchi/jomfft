#include "Dft.h"

#include "../../data/BluesteinDftData.h"
#include "../../model/ComplexCopy.h"
#include "../../model/ComplexFunctionBuffered.h"
#include "../Dft.h"
#include "../core/BatchMultiply.h"

#define NS_(n) NS(CDftBluestein_##n)

Long NS_(ConvolutionSize)(Long dft_size) {
  const Long u = 2 * dft_size - 1;
  Long out = 1;
  while (out < u) {
    out *= 2;
  }
  return out;
}

struct NS_(DftData) {
  Long dft_size;
  Long convolution_size;
  const struct NS(BluesteinDftData) *data;
  struct NS(ComplexFunction) *dft_f;
  struct NS(ComplexFunction) *dft_b;
  struct NS(ComplexFunction) *multiply_a0;
  struct NS(ComplexFunction) *multiply_a1;
  struct NS(ComplexFunction) *multiply_b;
};

static struct NS_(DftData) *NS_(DftData_new)(
    Long dft_size, Long convolution_size,
    const struct NS(BluesteinDftData) *data, struct NS(ComplexFunction) *dft_f,
    struct NS(ComplexFunction) *dft_b, struct NS(ComplexFunction) *multiply_a0,
    struct NS(ComplexFunction) *multiply_a1,
    struct NS(ComplexFunction) *multiply_b) {
  assert(data);
  assert(dft_f);
  assert(dft_b);
  assert(multiply_a0);
  assert(multiply_a1);
  assert(multiply_b);
  struct NS_(DftData) *out = NEW(struct NS_(DftData));
  *out = (struct NS_(DftData)){.dft_size = dft_size,
                               .convolution_size = convolution_size,
                               .data = data,
                               .dft_f = dft_f,
                               .dft_b = dft_b,
                               .multiply_a0 = multiply_a0,
                               .multiply_a1 = multiply_a1,
                               .multiply_b = multiply_b};
  return out;
}

static void NS_(DftData_destruct)(void *p) {
  if (p) {
    struct NS_(DftData) *o = (struct NS_(DftData) *)p;
    if (o->dft_f) {
      NS(ComplexFunction_delete)(o->dft_f);
      o->dft_f = NULL;
    }
    if (o->dft_b) {
      NS(ComplexFunction_delete)(o->dft_b);
      o->dft_b = NULL;
    }
    if (o->multiply_a0) {
      NS(ComplexFunction_delete)(o->multiply_a0);
      o->multiply_a0 = NULL;
    }
    if (o->multiply_a1) {
      NS(ComplexFunction_delete)(o->multiply_a1);
      o->multiply_a1 = NULL;
    }
    if (o->multiply_b) {
      NS(ComplexFunction_delete)(o->multiply_b);
      o->multiply_b = NULL;
    }
    DELETE(o);
  }
}

static void NS_(Dft_evaluate)(struct NS(ComplexFunctionW) *p, Complex *x,
                              Complex *y, void *work) {
  struct NS_(DftData) *data =
      (struct NS_(DftData) *)NS(ComplexFunctionW_data)(p);
  assert(data);
  Complex *z = (Complex *)work;
  const Long dft_size = data->dft_size;
  const Long convolution_size = data->convolution_size;
  NS(ComplexFunction_evaluate)(data->multiply_a0, x, z);
  ARRAY_ZERO(Complex, convolution_size - dft_size, z + dft_size);
  NS(ComplexFunction_evaluate)(data->dft_f, z, z);
  NS(ComplexFunction_evaluate)(data->multiply_b, z, z);
  NS(ComplexFunction_evaluate)(data->dft_b, z, z);
  NS(ComplexFunction_evaluate)(data->multiply_a1, z, y);
}

static Long NS_(Dft_size)(struct NS(ComplexFunctionW) *p) {
  struct NS_(DftData) *data =
      (struct NS_(DftData) *)NS(ComplexFunctionW_data)(p);
  assert(data);
  return data->convolution_size * sizeof(Complex);
}

struct NS(ComplexFunctionW) *NS_(Dft_newW)(struct NS(DataFactory) *data,
                                           bool backward, Long dft_size,
                                           Long x_stride, Long y_stride,
                                           int num_threads) {
#if PRINT_MODEL
  printf("Bluestein_Dft(%s,%ld;%d)\n", BOOL_STR(backward), dft_size,
         num_threads);
#endif
  const Long convolution_size = NS_(ConvolutionSize)(dft_size);
  const struct NS(BluesteinDftData) *dft_data =
      NS(DataFactory_bluesteinDftData)(data, dft_size, convolution_size);
  struct NS(ComplexFunction) *dft_f = NS(CDft_Dft_new)(
      data, backward, convolution_size, 1, 1, true, num_threads);
  struct NS(ComplexFunction) *dft_b = NS(CDft_Dft_new)(
      data, !backward, convolution_size, 1, 1, true, num_threads);
  struct NS(ComplexFunction) *multiply_a0 = NS(CDftCore_BatchMultiply_new)(
      backward, dft_size, dft_data->a, x_stride, 1, num_threads);
  struct NS(ComplexFunction) *multiply_a1 = NS(CDftCore_BatchMultiply_new)(
      backward, dft_size, dft_data->a, 1, y_stride, num_threads);
  struct NS(ComplexFunction) *multiply_b = NS(CDftCore_BatchMultiply_new)(
      backward, convolution_size, dft_data->b, 1, 1, num_threads);
  return NS(ComplexFunctionW_new)(
      NS_(Dft_evaluate), NS_(Dft_size), NS_(DftData_destruct),
      NS_(DftData_new)(dft_size, convolution_size, dft_data, dft_f, dft_b,
                       multiply_a0, multiply_a1, multiply_b));
}

struct NS(ComplexFunction) *NS_(Dft_new)(struct NS(DataFactory) *data,
                                         bool backward, Long dft_size,
                                         Long x_stride, Long y_stride,
                                         int num_threads) {
  return NS(ComplexFunctionBuffered_new)(
      NS_(Dft_newW)(data, backward, dft_size, x_stride, y_stride, num_threads));
}
