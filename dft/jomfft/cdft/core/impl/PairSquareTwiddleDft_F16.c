#include "PairSquareTwiddleDft_F16.h"

#define USE_COMPLEX_CONJ 0
#define CV_SIZE C_SIMD_SIZE_S
#define DFT_SIZE 16
#define FUNCTION NS(CDftCore_PairSquareTwiddleDft_F16)
#define TEMPLATE_FILE "Dft_16.h"

#include "../template/PairSquareTwiddleDft.h"
