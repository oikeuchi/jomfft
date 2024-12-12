#pragma once

#include <jomfft/model/ComplexFunction.h>

#include "../Test.h"

void NS(CDft_InnerBatchDftTest)(struct NS(ComplexFunction) *func, bool backward,
                                Long dft_size, Long batch_size, bool inplace);
