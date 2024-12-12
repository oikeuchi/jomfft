#include "RealTensor.h"

struct NS(RealTensor) *NS(Random_RealTensor_new)(int dimension,
                                                 const Long *sizes) {
  struct NS(RealTensor) *out = NS(RealTensor_new)(dimension, sizes);
  Real *it = NS(RealTensor_begin)(out);
  Real *end = NS(RealTensor_end)(out);
  for (; it != end; ++it) {
    *it = NS(Random_Real)();
  }
  return out;
}

struct NS(RealTensor) *NS(Random_RealTensor_new1)(Long size) {
  return NS(Random_RealTensor_new)(1, &size);
}

struct NS(RealTensor) *NS(Random_RealTensor_new2)(Long size0, Long size1) {
  Long sizes[] = {size0, size1};
  return NS(Random_RealTensor_new)(2, sizes);
}

struct NS(RealTensor) *NS(Random_RealTensor_new3)(Long size0, Long size1,
                                                  Long size2) {
  Long sizes[] = {size0, size1, size2};
  return NS(Random_RealTensor_new)(3, sizes);
}
