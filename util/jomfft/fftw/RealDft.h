#pragma once

#include <jomfft/model/ComplexToRealFunction.h>
#include <jomfft/model/RealToComplexFunction.h>

#include "../wrapper/RealDftSizes.h"
#include "Fftw.h"

struct NS(RealToComplexFunction) *NS(Fftw_RealDft_F_new)(
    const struct NS(Wrapper_RealDftSizes) *sizes, bool inplace,
    unsigned int flags);

struct NS(ComplexToRealFunction) *NS(Fftw_RealDft_B_new)(
    const struct NS(Wrapper_RealDftSizes) *sizes, bool inplace,
    unsigned int flags);
