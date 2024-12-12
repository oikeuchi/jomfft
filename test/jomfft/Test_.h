#pragma once

#ifdef _MSC_VER
#define USE_REGEX 0
#else
#include <regex.h>
#define USE_REGEX 1
#endif

#include <jomfft/Dft.h>
