#pragma once

#if JOMFFT_USE_FFTW
#include <fftw3.h>
#define USE_FFTW 1
#endif

#if JOMFFT_USE_MKL
#include <mkl.h>
#define USE_MKL 1
#endif

#include <jomfft/Dft.h>
