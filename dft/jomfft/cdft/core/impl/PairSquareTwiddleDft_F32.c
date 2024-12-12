#include "PairSquareTwiddleDft_F32.h"

#define USE_COMPLEX_CONJ 0
#define CV_SIZE C_SIMD_SIZE_S
#define DFT_SIZE 32
#define FUNCTION NS(CDftCore_PairSquareTwiddleDft_F32)
#define TEMPLATE_FILE "Dft_32.h"

#include "../template/PairSquareTwiddleDft.h"
