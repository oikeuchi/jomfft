#include "ConjugateEvenTensor.h"
#include "../TensorUtil.h"

static void Sample_Complex(int dimension, int i, const Long *sizes,
                           const Long *strides, Complex *data) {
  if (i == dimension - 1) {
    for (Long j = 0; j < sizes[i]; ++j) {
      data[j] = NS(Random_Complex)();
    }
  } else {
    for (Long j = 0; j < sizes[i]; ++j) {
      Sample_Complex(dimension, i + 1, sizes, strides, data + strides[i] * j);
    }
  }
}

static void Sample_Real(int dimension, int i, const Long *sizes,
                        const Long *strides, Complex *data) {
  if (i == dimension - 1) {
    for (Long j = 0; j < sizes[i]; ++j) {
      data[j] = Complex_R(NS(Random_Real)());
    }
  } else {
    for (Long j = 0; j < sizes[i]; ++j) {
      Sample_Complex(dimension, i + 1, sizes, strides, data + strides[i] * j);
    }
  }
}

static void Sample_ConjugateEven(int dimension, int conjugate_even_axis, int i,
                                 const Long *sizes, const Long *strides,
                                 Complex *data) {
  if (i == dimension - 1) {
    assert(i == conjugate_even_axis);
    const Long n = (sizes[i] + 1) / 2;
    data[0] = Complex_R(NS(Random_Real)());
    for (Long j = 1; j < n; ++j) {
      data[j] = NS(Random_Complex)();
    }
    if (sizes[i] % 2 == 0) {
      data[n] = Complex_R(NS(Random_Real)());
    }
  } else {
    if (i == conjugate_even_axis) {
      const Long n = (sizes[i] + 1) / 2;
      Sample_Real(dimension, i + 1, sizes, strides, data);
      for (Long j = 1; j < n; ++j) {
        Sample_Complex(dimension, i + 1, sizes, strides, data + strides[i] * j);
      }
      if (sizes[i] % 2 == 0) {
        Sample_Real(dimension, i + 1, sizes, strides, data + strides[i] * n);
      }
    } else {
      for (Long j = 0; j < sizes[i]; ++j) {
        Sample_ConjugateEven(dimension, conjugate_even_axis, i + 1, sizes,
                             strides, data + strides[i] * j);
      }
    }
  }
}

struct NS(ComplexTensor) *NS(Random_ConjugateEvenTensor_new_)(
    int dimension, const Long *sizes, int conjugate_even_axis) {
  assert(dimension >= 0);
  if (dimension == 0) {
    return NS(ComplexTensor_new)(dimension, sizes);
  }
  assert(conjugate_even_axis >= 0);
  assert(conjugate_even_axis < dimension);
  assert(sizes);
  Long *data_sizes = ARRAY_NEW(Long, dimension);
  ARRAY_COPY(Long, dimension, sizes, data_sizes);
  data_sizes[conjugate_even_axis] = CE_SIZE(sizes[conjugate_even_axis]);
  Long *strides = NS(Tensor_GetStrides)(dimension, data_sizes);
  struct NS(ComplexTensor) *out = NS(ComplexTensor_new)(dimension, data_sizes);
  Sample_ConjugateEven(dimension, conjugate_even_axis, 0, sizes, strides,
                       out->data);
  ARRAY_DELETE(data_sizes);
  ARRAY_DELETE(strides);
  return out;
}

struct NS(ComplexTensor) *NS(Random_ConjugateEvenTensor_new)(
    int dimension, const Long *sizes) {
  return NS(Random_ConjugateEvenTensor_new_)(dimension, sizes, dimension - 1);
}

struct NS(ComplexTensor) *NS(Random_ConjugateEvenTensor_new1)(Long size) {
  return NS(Random_ConjugateEvenTensor_new)(1, &size);
}

struct NS(ComplexTensor) *NS(Random_ConjugateEvenTensor_new2)(Long size0,
                                                              Long size1) {
  Long sizes[] = {size0, size1};
  return NS(Random_ConjugateEvenTensor_new)(2, sizes);
}

struct NS(ComplexTensor) *NS(Random_ConjugateEvenTensor_new3)(Long size0,
                                                              Long size1,
                                                              Long size2) {
  Long sizes[] = {size0, size1, size2};
  return NS(Random_ConjugateEvenTensor_new)(3, sizes);
}
