#include "TwiddleDft_B5.h"

#if C_SIMD_SIZE >= 2

#define USE_COMPLEX_CONJ 1
#define CV_SIZE 2
#define FUNCTION NS(CDftCore_TwiddleDft_B5_2)
#define TEMPLATE_FILE "Dft_5.h"

#include "../template/TwiddleDft.h"

#endif