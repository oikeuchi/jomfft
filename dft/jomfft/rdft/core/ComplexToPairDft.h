#pragma once

#include "../../model/ComplexFunction.h"
#include "Core.h"

struct NS(ComplexFunction) *NS(RDftCore_ComplexToPairDft_F_new)(
    Long dft_size, Long stride_0, Long stride_1, int num_threads);

struct NS(ComplexFunction) *NS(RDftCore_ComplexToPairDft_B_new)(
    Long dft_size, Long stride_0, Long stride_1, int num_threads);
