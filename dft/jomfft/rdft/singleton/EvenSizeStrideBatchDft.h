#pragma once

#include "../../data/DataFactory.h"
#include "../../model/ComplexToRealFunction.h"
#include "../../model/RealToComplexFunction.h"
#include "Singleton.h"

struct NS(RealToComplexFunction)
    *NS(RDftSingleton_EvenSizeStrideBatchDft_F_new)(
        struct NS(DataFactory) *data, int dft_size, Long batch_size,
        Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
        int num_threads);

struct NS(ComplexToRealFunction)
    *NS(RDftSingleton_EvenSizeStrideBatchDft_B_new)(
        struct NS(DataFactory) *data, int dft_size, Long batch_size,
        Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
        int num_threads);
