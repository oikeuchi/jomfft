#pragma once

#include <jomfft/model/ComplexToRealFunction.h>
#include <jomfft/model/RealToComplexFunction.h>

#include "../wrapper/RealDftSizes.h"
#include "Mkl.h"

struct NS(RealToComplexFunction) *NS(Mkl_RealDft_F_new)(
    const struct NS(Wrapper_RealDftSizes) *sizes, bool inplace);

struct NS(ComplexToRealFunction) *NS(Mkl_RealDft_B_new)(
    const struct NS(Wrapper_RealDftSizes) *sizes, bool inplace);
