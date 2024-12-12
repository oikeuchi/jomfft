#pragma once

#include "../Square.h"

void NS(CDftSquare_PairTwiddleTranspose_F)(Long size, const Complex *v,
                                           const Complex *w, Complex *x,
                                           int num_threads);
