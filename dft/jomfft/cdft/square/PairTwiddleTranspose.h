#pragma once

#include "Square.h"

struct NS(ComplexFunction) *NS(CDftSquare_PairTwiddleTranspose_new)(
    struct NS(DataFactory) *data, bool backward, Long size, int num_threads);
