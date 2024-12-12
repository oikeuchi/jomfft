#pragma once

#include "Square.h"

bool NS(RDftSquare_EvenSizeBatchDft_CanSplit)(
    const struct jomfftPrimeFactors *dft_factors, int num_threads);

struct NS(RealToComplexFunction) *NS(RDftSquare_EvenSizeBatchDft_F_new)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long batch_size, Long x_stride_0, Long x_stride_1, Long y_stride_0,
    Long y_stride_1, bool inplace, int num_threads);

struct NS(ComplexToRealFunction) *NS(RDftSquare_EvenSizeBatchDft_B_new)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long batch_size, Long x_stride_0, Long x_stride_1, Long y_stride_0,
    Long y_stride_1, bool inplace, int num_threads);
