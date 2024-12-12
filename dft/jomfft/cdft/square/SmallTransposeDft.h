#pragma once

#include <jomfft/util/IntVector.h>

#include "Square.h"

bool NS(CDftSquare_SmallTransposeDft_CanSplit)(
    const struct jomfftPrimeFactors *dft_factors);

struct NS(ComplexFunction) *NS(CDftSquare_SmallTransposeDft_new_)(
    struct NS(DataFactory) *data, bool backward, int square_dft_size,
    int inner_dft_size, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride);

struct NS(ComplexFunction) *NS(CDftSquare_SmallTransposeDft_new)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride);
