#pragma once

#include <jomfft/model/ComplexFunction.h>

#include "../Test.h"

void NS(CDft_TransposeDft_0Test)(struct NS(ComplexFunction) *func,
                                 bool backward, Long dft_size, Long batch_size);

void NS(CDft_TransposeDft_1Test)(struct NS(ComplexFunction) *func,
                                 bool backward, Long dft_size, Long batch_size);
