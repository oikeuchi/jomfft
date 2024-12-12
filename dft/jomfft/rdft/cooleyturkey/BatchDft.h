#pragma once

#include "CooleyTurkey.h"

struct NS(RealToComplexFunction) *NS(RDftCooleyTurkey_BatchDft_F_new)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long batch_size, Long x_stride_0, Long x_stride_1, Long y_stride_0,
    Long y_stride_1, int num_threads);

struct NS(ComplexToRealFunction) *NS(RDftCooleyTurkey_BatchDft_B_new)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long batch_size, Long x_stride_0, Long x_stride_1, Long y_stride_0,
    Long y_stride_1, int num_threads);
