#pragma once

#include "../data/DataFactory.h"
#include "../model/ComplexToRealFunction.h"
#include "../model/RealToComplexFunction.h"
#include "RDft.h"

struct NS(RealToComplexFunction) *NS(RDft_Dft_F_new)(
    struct NS(DataFactory) *data, Long dft_size, Long x_stride, Long y_stride,
    bool inplace, int num_threads);

struct NS(ComplexToRealFunction) *NS(RDft_Dft_B_new)(
    struct NS(DataFactory) *data, Long dft_size, Long x_stride, Long y_stride,
    bool inplace, int num_threads);
