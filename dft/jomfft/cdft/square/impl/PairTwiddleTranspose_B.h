#pragma once

#include "../Square.h"

void NS(CDftSquare_PairTwiddleTranspose_B)(Long size, const Complex *v,
                                           const Complex *w, Complex *x,
                                           int num_threads);
