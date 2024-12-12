#pragma once

#include "../Square.h"

void NS(CDftSquare_TwiddleTranspose_F)(Long size, const Complex *w, Complex *x,
                                       int num_threads);
