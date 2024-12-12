#include "TwiddleDft_B12.h"

#if C_SIMD_SIZE >= 8

#define USE_COMPLEX_CONJ 1
#define CV_SIZE 8
#define FUNCTION NS(CDftCore_TwiddleDft_B12_8)
#define TEMPLATE_FILE "Dft_12.h"

#include "../template/TwiddleDft.h"

#endif
