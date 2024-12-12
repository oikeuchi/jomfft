#pragma once

#include "../Util.h"

#if USE_SINGLE
#define FFTW_(n) fftwf_##n
#else
#define FFTW_(n) fftw_##n
#endif
