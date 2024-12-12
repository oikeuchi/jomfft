#include "ComplexTensor.h"

struct NS(ComplexTensor) *NS(Random_ComplexTensor_new)(int dimension,
                                                       const Long *sizes) {
  struct NS(ComplexTensor) *out = NS(ComplexTensor_new)(dimension, sizes);
  Complex *it = NS(ComplexTensor_begin)(out);
  Complex *end = NS(ComplexTensor_end)(out);
  for (; it != end; ++it) {
    *it = NS(Random_Complex)();
  }
  return out;
}

struct NS(ComplexTensor) *NS(Random_ComplexTensor_new1)(Long size) {
  return NS(Random_ComplexTensor_new)(1, &size);
}

struct NS(ComplexTensor) *NS(Random_ComplexTensor_new2)(Long size0,
                                                        Long size1) {
  Long sizes[] = {size0, size1};
  return NS(Random_ComplexTensor_new)(2, sizes);
}

struct NS(ComplexTensor) *NS(Random_ComplexTensor_new3)(Long size0, Long size1,
                                                        Long size2) {
  Long sizes[] = {size0, size1, size2};
  return NS(Random_ComplexTensor_new)(3, sizes);
}
