#include "TransposeDftTwiddle_F2.h"

#if C_SIMD_SIZE >= 8

#define USE_COMPLEX_CONJ 0
#define CV_SIZE 8
#define DFT_SIZE 2
#define FUNCTION NS(CDftCore_TransposeDftTwiddle_F2_8)

#include "../template/TransposeDftTwiddle_2_v.h"

#endif
