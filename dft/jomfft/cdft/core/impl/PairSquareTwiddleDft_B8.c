#include "PairSquareTwiddleDft_B8.h"

#define USE_COMPLEX_CONJ 1
#define CV_SIZE C_SIMD_SIZE_S
#define DFT_SIZE 8
#define FUNCTION NS(CDftCore_PairSquareTwiddleDft_B8)
#define TEMPLATE_FILE "Dft_8.h"

#include "../template/PairSquareTwiddleDft.h"
