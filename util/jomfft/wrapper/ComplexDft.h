#pragma once

#include <jomfft/model/ComplexFunction.h>

#include "ComplexDftSizes.h"
#include "Wrapper.h"

struct NS(ComplexFunction) *NS(Wrapper_ComplexDft_new_0)(bool backward,
                                                         int dimension,
                                                         const int *dft_sizes,
                                                         bool inplace);

struct NS(ComplexFunction) *NS(Wrapper_ComplexDft_new_1)(
    bool backward, const struct NS(Wrapper_ComplexDftSizes) *sizes,
    bool inplace);

struct NS(ComplexFunction) *NS(Wrapper_ComplexDft_new_2)(
    bool backward, const struct NS(Wrapper_ComplexDftSizes) *sizes,
    bool inplace);
