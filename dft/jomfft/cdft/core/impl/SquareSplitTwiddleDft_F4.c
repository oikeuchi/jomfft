#include "SquareSplitTwiddleDft_F4.h"

#define USE_COMPLEX_CONJ 0
#define CV_SIZE MIN(C_SIMD_SIZE, 4)
#define DFT_SIZE 4
#define FUNCTION NS(CDftCore_SquareSplitTwiddleDft_F4)
#define TEMPLATE_FILE "Dft_4.h"

#include "../template/SquareSplitTwiddleDft.h"
