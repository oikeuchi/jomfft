#include "TwiddleTranspose_B.h"

#define USE_COMPLEX_CONJ 1
#define CV_SIZE C_SIMD_SIZE
#define FUNCTION NS(CDftSquare_TwiddleTranspose_B)

#include "../template/TwiddleTranspose.h"
