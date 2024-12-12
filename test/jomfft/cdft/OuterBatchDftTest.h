#pragma once

#include <jomfft/model/ComplexFunction.h>

#include "../Test.h"

void NS(CDft_OuterBatchDftTest)(struct NS(ComplexFunction) *func, bool backward,
                                Long dft_size, Long batch_size, Long x_stride,
                                Long y_stride, bool inplace);
