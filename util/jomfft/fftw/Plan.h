#pragma once

#include "Fftw.h"

#if USE_FFTW

FFTW_(plan) * NS(Fftw_Plan_new)();

void NS(Fftw_Plan_destruct)(void *p);

void NS(Fftw_Plan_print)(FFTW_(plan) * p);

#endif
