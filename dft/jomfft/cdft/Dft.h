#pragma once

#include "../data/DataFactory.h"
#include "../model/ComplexFunction.h"
#include "CDft.h"

struct NS(ComplexFunction) *NS(CDft_Dft_new)(struct NS(DataFactory) *data,
                                             bool backward, Long dft_size,
                                             Long x_stride, Long y_stride,
                                             bool inplace, int num_threads);
