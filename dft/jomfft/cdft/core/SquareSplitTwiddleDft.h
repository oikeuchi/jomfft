#pragma once

#include "../../data/DataFactory.h"
#include "../../model/ComplexFunction.h"
#include "Core.h"

struct NS(ComplexFunction) *NS(CDftCore_SquareSplitTwiddleDft_new)(
    struct NS(DataFactory) *data, bool backward, int dft_size,
    Long twiddle_size, int num_threads);
