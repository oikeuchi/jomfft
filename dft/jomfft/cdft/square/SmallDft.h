#pragma once

#include "Square.h"

bool NS(CDftSquare_SmallDft_CanSplit)(
    const struct jomfftPrimeFactors *dft_factors);

struct NS(ComplexFunction) *NS(CDftSquare_SmallDft_new_1)(
    struct NS(DataFactory) *data, bool backward, int square_dft_size,
    int inner_dft_size);

struct NS(ComplexFunction) *NS(CDftSquare_SmallDft_new)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long x_stride, Long y_stride,
    bool inplace);
