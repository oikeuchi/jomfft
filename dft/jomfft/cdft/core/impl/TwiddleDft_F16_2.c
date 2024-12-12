#include "TwiddleDft_F16.h"

#if C_SIMD_SIZE >= 2

#define USE_COMPLEX_CONJ 0
#define CV_SIZE 2
#define FUNCTION NS(CDftCore_TwiddleDft_F16_2)
#define TEMPLATE_FILE "Dft_16.h"

#include "../template/TwiddleDft.h"

#endif
