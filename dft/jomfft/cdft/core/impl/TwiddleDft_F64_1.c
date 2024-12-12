#include "TwiddleDft_F64.h"

#if C_SIMD_SIZE >= 1

#define USE_COMPLEX_CONJ 0
#define CV_SIZE 1
#define FUNCTION NS(CDftCore_TwiddleDft_F64_1)
#define TEMPLATE_FILE "Dft_64.h"

#include "../template/TwiddleDft.h"

#endif
