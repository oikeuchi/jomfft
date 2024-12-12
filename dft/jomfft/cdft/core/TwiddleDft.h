#pragma once

#include "../../data/DataFactory.h"
#include "../../model/ComplexFunction.h"
#include "Core.h"
#include <jomfft/util/IntVector.h>

bool NS(CDftCore_TwiddleDft_HasKey)(int dft_size);

struct jomfftIntVector *NS(CDftCore_TwiddleDft_Keys)();

struct NS(ComplexFunction) *NS(CDftCore_TwiddleDft_new)(
    struct NS(DataFactory) *data, bool backward, int dft_size,
    Long twiddle_size);

struct NS(ComplexFunction) *NS(CDftCore_TwiddleDft_new_)(
    struct NS(DataFactory) *data, bool backward, int dft_size,
    Long twiddle_size, int num_threads);
