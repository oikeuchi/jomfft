#pragma once

#include "CooleyTurkey.h"
#include <jomfft/util/IntVector.h>

struct jomfftIntVector *NS(CDftCooleyTurkey_Dft_Split)(
    const struct jomfftPrimeFactors *factors, int num_threads);

struct NS(ComplexFunction) *NS(CDftCooleyTurkey_Dft_new_1)(
    struct NS(DataFactory) *data, bool backward, Long dimension,
    const int *dft_sizes, Long x_stride, int num_threads);

struct NS(ComplexFunction) *NS(CDftCooleyTurkey_Dft_new)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long x_stride, Long y_stride,
    bool inplace, int num_threads);

struct NS(ComplexFunctionW) *NS(CDftCooleyTurkey_Dft_newW)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long x_stride, Long y_stride,
    int num_threads);
