#pragma once

#include <jomfft/model/ComplexToRealFunction.h>
#include <jomfft/model/RealToComplexFunction.h>

#include "RealDftSizes.h"
#include "Wrapper.h"

struct NS(RealToComplexFunction) *NS(Wrapper_RealDft_F_new_0)(
    int dimension, const int *dft_sizes, bool inplace);

struct NS(RealToComplexFunction) *NS(Wrapper_RealDft_F_new_1)(
    const struct NS(Wrapper_RealDftSizes) *sizes, bool inplace);

struct NS(RealToComplexFunction) *NS(Wrapper_RealDft_F_new_2)(
    const struct NS(Wrapper_RealDftSizes) *sizes, bool inplace);

struct NS(ComplexToRealFunction) *NS(Wrapper_RealDft_B_new_0)(
    int dimension, const int *dft_sizes, bool inplace);

struct NS(ComplexToRealFunction) *NS(Wrapper_RealDft_B_new_1)(
    const struct NS(Wrapper_RealDftSizes) *sizes, bool inplace);

struct NS(ComplexToRealFunction) *NS(Wrapper_RealDft_B_new_2)(
    const struct NS(Wrapper_RealDftSizes) *sizes, bool inplace);
