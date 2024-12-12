#pragma once

#include "../data/DataFactory.h"
#include "../model/ComplexFunction.h"
#include "CDft.h"

struct NS(ComplexFunction) *NS(CDft_MainDft_new)(
    struct NS(DataFactory) *data, bool backward, Long dimension,
    const Long *dft_sizes, Long batch_size, const Long *x_strides,
    Long x_distance, const Long *y_strides, Long y_distance, bool inplace,
    int num_threads);

struct NS(ComplexFunction) *NS(CDft_BatchDftLoop_new)(
    struct NS(DataFactory) *data, bool backward, Long dft_size, Long x_distance,
    Long dimension, const Long *sizes, const Long *x_strides, int num_threads);
