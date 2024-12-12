#pragma once

#include <jomfft/model/ComplexFunction.h>

#include "../wrapper/ComplexDftSizes.h"
#include "Fftw.h"

struct NS(ComplexFunction) *NS(Fftw_ComplexDft_new)(
    bool backward, const struct NS(Wrapper_ComplexDftSizes) *sizes,
    bool inplace, unsigned int flags);
