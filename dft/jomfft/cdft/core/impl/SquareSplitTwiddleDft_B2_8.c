#include "SquareSplitTwiddleDft_B2.h"

#if C_SIMD_SIZE >= 8

#define USE_COMPLEX_CONJ 1
#define CV_SIZE 8
#define DFT_SIZE 2
#define FUNCTION NS(CDftCore_SquareSplitTwiddleDft_B2_8)
#define TEMPLATE_FILE "Dft_2.h"

#include "../template/SquareSplitTwiddleDft_v.h"

#endif
