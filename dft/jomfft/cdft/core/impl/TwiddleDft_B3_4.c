#include "TwiddleDft_B3.h"

#if C_SIMD_SIZE >= 4

#define USE_COMPLEX_CONJ 1
#define CV_SIZE 4
#define FUNCTION NS(CDftCore_TwiddleDft_B3_4)
#define TEMPLATE_FILE "Dft_3.h"

#include "../template/TwiddleDft.h"

#endif