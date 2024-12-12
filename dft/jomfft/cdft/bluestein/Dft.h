#pragma once

#include "../../model/ComplexFunctionW.h"
#include "Bluestein.h"

Long NS(CDftBluestein_ConvolutionSize)(Long dft_size);

struct NS(ComplexFunctionW) *NS(CDftBluestein_Dft_newW)(
    struct NS(DataFactory) *data, bool backward, Long dft_size, Long x_stride,
    Long y_stride, int num_threads);

struct NS(ComplexFunction) *NS(CDftBluestein_Dft_new)(
    struct NS(DataFactory) *data, bool backward, Long dft_size, Long x_stride,
    Long y_stride, int num_threads);
