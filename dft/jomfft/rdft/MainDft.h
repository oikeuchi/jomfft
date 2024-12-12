#pragma once

#include "../data/DataFactory.h"
#include "../model/ComplexToRealFunction.h"
#include "../model/RealToComplexFunction.h"
#include "RDft.h"

struct NS(RealToComplexFunction) *NS(RDft_MainDft_F_new)(
    struct NS(DataFactory) *data, Long dimension, const Long *dft_sizes,
    Long batch_size, const Long *x_strides, Long x_distance,
    const Long *y_strides, Long y_distance, bool inplace, int num_threads);

struct NS(ComplexToRealFunction) *NS(RDft_MainDft_B_new)(
    struct NS(DataFactory) *data, Long dimension, const Long *dft_sizes,
    Long batch_size, const Long *x_strides, Long x_distance,
    const Long *y_strides, Long y_distance, bool inplace, int num_threads);
