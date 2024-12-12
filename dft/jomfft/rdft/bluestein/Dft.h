#pragma once

#include "../../model/ComplexToRealFunctionW.h"
#include "../../model/RealToComplexFunctionW.h"
#include "Bluestein.h"

struct NS(RealToComplexFunctionW) *NS(RDftBluestein_Dft_F_newW)(
    struct NS(DataFactory) *data, Long dft_size, Long x_stride, Long y_stride,
    int num_threads);

struct NS(ComplexToRealFunctionW) *NS(RDftBluestein_Dft_B_newW)(
    struct NS(DataFactory) *data, Long dft_size, Long x_stride, Long y_stride,
    int num_threads);

struct NS(RealToComplexFunction) *NS(RDftBluestein_Dft_F_new)(
    struct NS(DataFactory) *data, Long dft_size, Long x_stride, Long y_stride,
    int num_threads);

struct NS(ComplexToRealFunction) *NS(RDftBluestein_Dft_B_new)(
    struct NS(DataFactory) *data, Long dft_size, Long x_stride, Long y_stride,
    int num_threads);
