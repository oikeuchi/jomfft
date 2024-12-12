#pragma once

#include "../../data/DataFactory.h"
#include "../../model/ComplexFunction.h"
#include "Singleton.h"

struct NS(ComplexFunction) *NS(CDftSingleton_StrideBatchDft_new)(
    struct NS(DataFactory) *data, bool backward, int dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1);

struct NS(ComplexFunction) *NS(CDftSingleton_StrideBatchDft_new_)(
    struct NS(DataFactory) *data, bool backward, int dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads);
