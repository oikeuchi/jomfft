#pragma once

#include "Square.h"

bool NS(CDftSquare_Dft_CanSplit)(const struct jomfftPrimeFactors *dft_factors,
                                 int num_threads);

struct NS(ComplexFunction) *NS(CDftSquare_Dft_new)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long x_stride, Long y_stride,
    bool inplace, int num_threads);
