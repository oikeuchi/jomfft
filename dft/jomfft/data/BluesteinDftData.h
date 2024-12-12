#pragma once

#include "Data.h"

struct NS(DataFactory);

struct NS(BluesteinDftDataKey) {
  Long dft_size;
  Long convolution_size;
};

bool NS(BluesteinDftDataKey_equal)(struct NS(BluesteinDftDataKey) a,
                                   struct NS(BluesteinDftDataKey) b);

struct NS(BluesteinDftData) {
  const Complex *a;
  const Complex *b;
};

struct NS(BluesteinDftData) *NS(BluesteinDftData_new)(
    struct NS(DataFactory) *data, struct NS(BluesteinDftDataKey) key);

void NS(BluesteinDftData_delete)(struct NS(BluesteinDftData) *p);
