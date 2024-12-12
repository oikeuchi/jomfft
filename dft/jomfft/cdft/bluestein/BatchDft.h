#pragma once

#include "Bluestein.h"

struct NS(ComplexFunction) *NS(CDftBluestein_BatchDft_new)(
    struct NS(DataFactory) *data, bool backward, Long dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads);
