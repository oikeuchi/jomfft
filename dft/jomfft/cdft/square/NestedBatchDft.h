#pragma once

#include "Square.h"

bool NS(CDftSquare_NestedBatchDft_CanSplit)(
    const struct jomfftPrimeFactors *dft_factors);

struct NS(ComplexFunction) *NS(CDftSquare_NestedBatchDft_new)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    bool inplace, int num_threads);
