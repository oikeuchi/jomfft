#pragma once

#include "CDft.h"

Long NS(CDft_IsBadStrideForCache)(Long dft_size, Long stride);

Long NS(CDft_BatchChunkSize)(Long dft_size, Long batch_size, Long x_stride,
                             Long y_stride);
