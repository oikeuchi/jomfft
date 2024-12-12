#pragma once

#include "../data/DataFactory.h"
#include "../model/ComplexFunction.h"
#include "../model/ComplexFunctionW.h"
#include "../model/ComplexToRealFunction.h"
#include "../model/ComplexToRealFunctionW.h"
#include "../model/RealToComplexFunction.h"
#include "../model/RealToComplexFunctionW.h"
#include "RDft.h"

struct NS(RealToComplexFunction) *NS(RDft_EvenSizeDft_F_new)(
    struct NS(DataFactory) *data, struct NS(ComplexFunction) *dft,
    Long dft_size, int num_threads);

struct NS(ComplexToRealFunction) *NS(RDft_EvenSizeDft_B_new)(
    struct NS(DataFactory) *data, struct NS(ComplexFunction) *dft,
    Long dft_size, int num_threads);

struct NS(RealToComplexFunctionW) *NS(RDft_EvenSizeDftW_F_new)(
    struct NS(DataFactory) *data, struct NS(ComplexFunctionW) *dft,
    Long dft_size, int num_threads);

struct NS(ComplexToRealFunctionW) *NS(RDft_EvenSizeDftW_B_new)(
    struct NS(DataFactory) *data, struct NS(ComplexFunctionW) *dft,
    Long dft_size, int num_threads);

struct NS(RealToComplexFunction) *NS(RDft_EvenSizeOuterBatchDft_F_new)(
    struct NS(DataFactory) *data, struct NS(ComplexFunction) *dft,
    Long dft_size, Long batch_size, Long stride);

struct NS(ComplexToRealFunction) *NS(RDft_EvenSizeOuterBatchDft_B_new)(
    struct NS(DataFactory) *data, struct NS(ComplexFunction) *dft,
    Long dft_size, Long batch_size, Long stride);
