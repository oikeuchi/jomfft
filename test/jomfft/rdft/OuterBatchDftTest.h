#pragma once

#include <jomfft/model/ComplexToRealFunction.h>
#include <jomfft/model/RealToComplexFunction.h>

#include "../Test.h"

void NS(RDft_OuterBatchDft_FTest)(struct NS(RealToComplexFunction) *func,
                                  Long dft_size, Long batch_size, Long x_stride,
                                  Long y_stride, bool inplace);

void NS(RDft_OuterBatchDft_BTest)(struct NS(ComplexToRealFunction) *func,
                                  Long dft_size, Long batch_size, Long x_stride,
                                  Long y_stride, bool inplace);
