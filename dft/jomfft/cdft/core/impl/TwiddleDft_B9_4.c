#include "TwiddleDft_B9.h"

#if C_SIMD_SIZE >= 4

#define USE_COMPLEX_CONJ 1
#define CV_SIZE 4
#define FUNCTION NS(CDftCore_TwiddleDft_B9_4)
#define TEMPLATE_FILE "Dft_9.h"

#include "../template/TwiddleDft.h"

#endif
