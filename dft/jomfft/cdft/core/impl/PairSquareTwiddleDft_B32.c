#include "PairSquareTwiddleDft_B32.h"

#define USE_COMPLEX_CONJ 1
#define CV_SIZE C_SIMD_SIZE_S
#define DFT_SIZE 32
#define FUNCTION NS(CDftCore_PairSquareTwiddleDft_B32)
#define TEMPLATE_FILE "Dft_32.h"

#include "../template/PairSquareTwiddleDft.h"
