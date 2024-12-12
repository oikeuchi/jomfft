#include "TwiddleDft_F10.h"

#if C_SIMD_SIZE >= 2

#define USE_COMPLEX_CONJ 0
#define CV_SIZE 2
#define FUNCTION NS(CDftCore_TwiddleDft_F10_2)
#define TEMPLATE_FILE "Dft_10.h"

#include "../template/TwiddleDft.h"

#endif
