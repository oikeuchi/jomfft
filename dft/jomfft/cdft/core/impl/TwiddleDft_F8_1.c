#include "TwiddleDft_F8.h"

#if C_SIMD_SIZE >= 1

#define USE_COMPLEX_CONJ 0
#define CV_SIZE 1
#define FUNCTION NS(CDftCore_TwiddleDft_F8_1)
#define TEMPLATE_FILE "Dft_8.h"

#include "../template/TwiddleDft.h"

#endif
