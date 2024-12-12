#pragma once

#include "../../model/ComplexToRealFunctionW.h"
#include "../../model/RealToComplexFunctionW.h"
#include "CooleyTurkey.h"

struct NS(RealToComplexFunctionW) *NS(RDftCooleyTurkey_Dft_F_newW)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long x_stride, Long y_stride, int num_threads);

struct NS(ComplexToRealFunctionW) *NS(RDftCooleyTurkey_Dft_B_newW)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long x_stride, Long y_stride, int num_threads);

struct NS(RealToComplexFunction) *NS(RDftCooleyTurkey_Dft_F_new)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long x_stride, Long y_stride, int num_threads);

struct NS(ComplexToRealFunction) *NS(RDftCooleyTurkey_Dft_B_new)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long x_stride, Long y_stride, int num_threads);
