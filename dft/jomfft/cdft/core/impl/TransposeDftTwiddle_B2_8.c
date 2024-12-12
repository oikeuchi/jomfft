#include "TransposeDftTwiddle_B2.h"

#if C_SIMD_SIZE >= 8

#define USE_COMPLEX_CONJ 1
#define CV_SIZE 8
#define DFT_SIZE 2
#define FUNCTION NS(CDftCore_TransposeDftTwiddle_B2_8)

#include "../template/TransposeDftTwiddle_2_v.h"

#endif
