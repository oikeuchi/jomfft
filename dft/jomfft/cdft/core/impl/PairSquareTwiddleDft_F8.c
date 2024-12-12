#include "PairSquareTwiddleDft_F8.h"

#define USE_COMPLEX_CONJ 0
#define CV_SIZE C_SIMD_SIZE_S
#define DFT_SIZE 8
#define FUNCTION NS(CDftCore_PairSquareTwiddleDft_F8)
#define TEMPLATE_FILE "Dft_8.h"

#include "../template/PairSquareTwiddleDft.h"
