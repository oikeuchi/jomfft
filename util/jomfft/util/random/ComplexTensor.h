#pragma once

#include "../ComplexTensor.h"
#include "Random.h"

struct NS(ComplexTensor) *NS(Random_ComplexTensor_new)(int dimension,
                                                       const Long *sizes);
struct NS(ComplexTensor) *NS(Random_ComplexTensor_new1)(Long size);
struct NS(ComplexTensor) *NS(Random_ComplexTensor_new2)(Long size0, Long size1);
struct NS(ComplexTensor) *NS(Random_ComplexTensor_new3)(Long size0, Long size1,
                                                        Long size2);
