#pragma once

#include <jomfft/model/ComplexToRealFunction.h>
#include <jomfft/model/RealToComplexFunction.h>

#include "../Test.h"

void NS(RDft_InnerBatchDft_FTest)(struct NS(RealToComplexFunction) *func,
                                  Long dft_size, Long batch_size);

void NS(RDft_InnerBatchDft_BTest)(struct NS(ComplexToRealFunction) *func,
                                  Long dft_size, Long batch_size);
