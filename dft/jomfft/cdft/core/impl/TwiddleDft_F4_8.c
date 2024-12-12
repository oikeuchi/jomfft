#include "TwiddleDft_F4.h"

#if C_SIMD_SIZE >= 8

#define USE_COMPLEX_CONJ 0
#define CV_SIZE 8
#define FUNCTION NS(CDftCore_TwiddleDft_F4_8)
#define TEMPLATE_FILE "Dft_4.h"

#include "../template/TwiddleDft.h"

#endif
