#pragma once

#include "../Square.h"

void NS(CDftSquare_TwiddleTranspose_B)(Long size, const Complex *w, Complex *x,
                                       int num_threads);
