#pragma once

#include "Square.h"

bool NS(CDftSquare_LargeDft_CanSplit)(
    const struct jomfftPrimeFactors *dft_factors);

struct NS(ComplexFunction) *NS(CDftSquare_LargeDft_new_1)(
    struct NS(DataFactory) *data, bool backward, Long square_dft_size,
    int inner_dft_size, Long x_stride, Long y_stride, bool inplace,
    int num_threads);

struct NS(ComplexFunction) *NS(CDftSquare_LargeDft_new)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long x_stride, Long y_stride,
    bool inplace, int num_threads);
