#include "TwiddleTranspose_F.h"

#define USE_COMPLEX_CONJ 0
#define CV_SIZE C_SIMD_SIZE
#define FUNCTION NS(CDftSquare_TwiddleTranspose_F)

#include "../template/TwiddleTranspose.h"
