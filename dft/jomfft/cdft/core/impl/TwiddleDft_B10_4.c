#include "TwiddleDft_B10.h"

#if C_SIMD_SIZE >= 4

#define USE_COMPLEX_CONJ 1
#define CV_SIZE 4
#define FUNCTION NS(CDftCore_TwiddleDft_B10_4)
#define TEMPLATE_FILE "Dft_10.h"

#include "../template/TwiddleDft.h"

#endif
