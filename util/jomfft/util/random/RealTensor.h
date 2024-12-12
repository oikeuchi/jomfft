#pragma once

#include "../RealTensor.h"
#include "Random.h"

struct NS(RealTensor) *NS(Random_RealTensor_new)(int dimension,
                                                 const Long *sizes);
struct NS(RealTensor) *NS(Random_RealTensor_new1)(Long size);
struct NS(RealTensor) *NS(Random_RealTensor_new2)(Long size0, Long size1);
struct NS(RealTensor) *NS(Random_RealTensor_new3)(Long size0, Long size1,
                                                  Long size2);
