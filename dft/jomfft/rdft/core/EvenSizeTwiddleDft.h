#pragma once

#include "../../data/DataFactory.h"
#include "../../model/ComplexFunction.h"
#include "Core.h"

struct NS(ComplexFunction) *NS(RDftCore_EvenSizeTwiddleDft_F_new)(
    struct NS(DataFactory) *data, Long dft_size);

struct NS(ComplexFunction) *NS(RDftCore_EvenSizeTwiddleDft_B_new)(
    struct NS(DataFactory) *data, Long dft_size);

struct NS(ComplexFunction) *NS(RDftCore_EvenSizeTwiddleDft_F_new_)(
    struct NS(DataFactory) *data, Long dft_size, int num_threads);

struct NS(ComplexFunction) *NS(RDftCore_EvenSizeTwiddleDft_B_new_)(
    struct NS(DataFactory) *data, Long dft_size, int num_threads);
