#include "TwiddleDft_F2.h"

#if C_SIMD_SIZE >= 1

#define USE_COMPLEX_CONJ 0
#define CV_SIZE 1
#define FUNCTION NS(CDftCore_TwiddleDft_F2_1)
#define TEMPLATE_FILE "Dft_2.h"

#include "../template/TwiddleDft.h"

#endif
