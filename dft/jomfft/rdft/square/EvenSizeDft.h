#pragma once

#include "Square.h"

bool NS(RDftSquare_EvenSizeDft_CanSplit)(
    const struct jomfftPrimeFactors *dft_factors, int num_threads);

struct NS(RealToComplexFunction) *NS(RDftSquare_EvenSizeDft_F_new_1)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    bool inplace, int num_threads);

struct NS(ComplexToRealFunction) *NS(RDftSquare_EvenSizeDft_B_new_1)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    bool inplace, int num_threads);

struct NS(RealToComplexFunction) *NS(RDftSquare_EvenSizeDft_F_new)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long x_stride, Long y_stride, bool inplace, int num_threads);

struct NS(ComplexToRealFunction) *NS(RDftSquare_EvenSizeDft_B_new)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long x_stride, Long y_stride, bool inplace, int num_threads);

struct NS(RealToComplexFunctionW) *NS(RDftSquare_EvenSizeDft_F_newW)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long x_stride, Long y_stride, int num_threads);

struct NS(ComplexToRealFunctionW) *NS(RDftSquare_EvenSizeDft_B_newW)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long x_stride, Long y_stride, int num_threads);
