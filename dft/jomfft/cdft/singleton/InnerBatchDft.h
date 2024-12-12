#pragma once

#include "../../data/DataFactory.h"
#include "../../model/ComplexFunction.h"
#include "Singleton.h"

struct NS(ComplexFunction) *NS(CDftSingleton_InnerBatchDft_new)(
    struct NS(DataFactory) *data, bool backward, int dft_size, Long batch_size,
    Long x_stride, Long y_stride, int num_threads);
