#include "TwiddleDft_B16.h"

#if C_SIMD_SIZE >= 1

#define USE_COMPLEX_CONJ 1
#define CV_SIZE 1
#define FUNCTION NS(CDftCore_TwiddleDft_B16_1)
#define TEMPLATE_FILE "Dft_16.h"

#include "../template/TwiddleDft.h"

#endif