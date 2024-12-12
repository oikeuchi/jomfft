#include "SquareSplitTwiddleDft_F8.h"

#define USE_COMPLEX_CONJ 0
#define CV_SIZE C_SIMD_SIZE
#define DFT_SIZE 8
#define FUNCTION NS(CDftCore_SquareSplitTwiddleDft_F8)
#define TEMPLATE_FILE "Dft_8.h"

#include "../template/SquareSplitTwiddleDft.h"
