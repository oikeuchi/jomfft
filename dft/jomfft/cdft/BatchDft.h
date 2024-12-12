#pragma once

#include "../data/DataFactory.h"
#include "../model/ComplexFunction.h"
#include "CDft.h"

struct NS(ComplexFunction) *NS(CDft_BatchDft_new)(
    struct NS(DataFactory) *data, bool backward, Long dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    bool inplace, int num_threads);
