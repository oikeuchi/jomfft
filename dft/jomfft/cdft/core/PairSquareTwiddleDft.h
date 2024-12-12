#pragma once

#include "../../data/DataFactory.h"
#include "../../model/ComplexFunction.h"
#include "Core.h"

bool NS(CDftCore_PairSquareTwiddleDft_HasKey)(int dft_size);

struct NS(ComplexFunction) *NS(CDftCore_PairSquareTwiddleDft_new)(
    struct NS(DataFactory) *data, bool backward, int dft_size);
