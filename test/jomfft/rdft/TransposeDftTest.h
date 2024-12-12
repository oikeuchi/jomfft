#pragma once

#include <jomfft/model/ComplexToRealFunction.h>
#include <jomfft/model/RealToComplexFunction.h>

#include "../Test.h"

void NS(RDft_TransposeDft_0_FTest)(struct NS(RealToComplexFunction) *func,
                                   Long dft_size, Long batch_size);

void NS(RDft_TransposeDft_1_FTest)(struct NS(RealToComplexFunction) *func,
                                   Long dft_size, Long batch_size);

void NS(RDft_TransposeDft_0_BTest)(struct NS(ComplexToRealFunction) *func,
                                   Long dft_size, Long batch_size);

void NS(RDft_TransposeDft_1_BTest)(struct NS(ComplexToRealFunction) *func,
                                   Long dft_size, Long batch_size);
