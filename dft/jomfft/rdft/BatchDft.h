#pragma once

#include "../data/DataFactory.h"
#include "../model/ComplexToRealFunction.h"
#include "../model/RealToComplexFunction.h"
#include "RDft.h"

struct NS(RealToComplexFunction) *NS(RDft_BatchDft_F_new)(
    struct NS(DataFactory) *data, Long dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    bool inplace, int num_threads);

struct NS(ComplexToRealFunction) *NS(RDft_BatchDft_B_new)(
    struct NS(DataFactory) *data, Long dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    bool inplace, int num_threads);
