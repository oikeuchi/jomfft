#pragma once

#include "CooleyTurkey.h"

struct NS(ComplexFunction) *NS(CDftCooleyTurkey_BatchDft_new)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    bool inplace, int num_threads);

bool NS(CDftCooleyTurkey_UseChunkedBatch)(
    const struct jomfftPrimeFactors *dft_factors, Long batch_size,
    Long x_stride_0, Long y_stride_0);
