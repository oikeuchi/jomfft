#pragma once

#include "Square.h"

bool NS(CDftSquare_NestedDft_CanSplit)(
    const struct jomfftPrimeFactors *dft_factors);

struct NS(ComplexFunction) *NS(CDftSquare_NestedDft_new_1)(
    struct NS(DataFactory) *data, bool backward, int square_dft_size,
    const struct jomfftPrimeFactors *inner_dft_factors, Long x_stride,
    int num_threads);

struct NS(ComplexFunction) *NS(CDftSquare_NestedDft_new)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long x_stride, Long y_stride,
    bool inplace, int num_threads);
