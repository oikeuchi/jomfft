#include "TwiddleDft_F3.h"

#if C_SIMD_SIZE >= 4

#define USE_COMPLEX_CONJ 0
#define CV_SIZE 4
#define FUNCTION NS(CDftCore_TwiddleDft_F3_4)
#define TEMPLATE_FILE "Dft_3.h"

#include "../template/TwiddleDft.h"

#endif