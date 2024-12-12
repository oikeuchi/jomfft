#pragma once

#include <jomfft/util/IntVector.h>

#include "CooleyTurkey.h"

struct jomfftIntVector *NS(CDftCooleyTurkey_TransposeDft_Split)(
    const struct jomfftPrimeFactors *dft_factors);

struct NS(ComplexFunction) *NS(CDftCooleyTurkey_TransposeDft_new_)(
    struct NS(DataFactory) *data, bool backward, Long dimension,
    const int *dft_sizes, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride);

struct NS(ComplexFunction) *NS(CDftCooleyTurkey_TransposeDft_new)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride);
