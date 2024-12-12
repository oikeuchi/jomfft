#pragma once

#include "Bluestein.h"

struct NS(RealToComplexFunction) *NS(RDftBluestein_EvenSizeBatchDft_F_new)(
    struct NS(DataFactory) *data, Long dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads);

struct NS(ComplexToRealFunction) *NS(RDftBluestein_EvenSizeBatchDft_B_new)(
    struct NS(DataFactory) *data, Long dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads);
