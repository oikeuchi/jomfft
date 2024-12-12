#include "SquareSplitTwiddleDft_F2.h"

#define USE_COMPLEX_CONJ 0
#define CV_SIZE MIN(C_SIMD_SIZE, 2)
#define DFT_SIZE 2
#define FUNCTION NS(CDftCore_SquareSplitTwiddleDft_F2)
#define TEMPLATE_FILE "Dft_2.h"

#include "../template/SquareSplitTwiddleDft.h"
