#pragma once

#include <jomfft/model/ComplexFunction.h>

#include "../wrapper/ComplexDftSizes.h"
#include "Mkl.h"

struct NS(ComplexFunction) *NS(Mkl_ComplexDft_new)(
    bool backward, const struct NS(Wrapper_ComplexDftSizes) *sizes,
    bool inplace);