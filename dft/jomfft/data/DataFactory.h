#pragma once

#include "Data.h"

struct NS(DataFactory);
struct NS(BluesteinDftData);

struct NS(DataFactory) *NS(DataFactory_new)();

void NS(DataFactory_delete)(struct NS(DataFactory) *p);

const Complex *NS(DataFactory_twiddleVector)(struct NS(DataFactory) *p,
                                             Long size, Long twiddle_size,
                                             Long first_index);

const Complex *NS(DataFactory_twiddleMatrix)(struct NS(DataFactory) *p,
                                             Long size_0, Long size_1,
                                             Long twiddle_size,
                                             Long first_index_0,
                                             Long first_index_1);

const Complex *NS(DataFactory_largeTwiddleMatrix)(struct NS(DataFactory) *p,
                                                  Long size, Long twiddle_size,
                                                  int block_size);

const struct NS(BluesteinDftData) *NS(DataFactory_bluesteinDftData)(
    struct NS(DataFactory) *p, Long dft_size, Long convolution_size);
