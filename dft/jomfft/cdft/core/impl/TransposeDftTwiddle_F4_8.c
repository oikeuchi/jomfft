#include "TransposeDftTwiddle_F4.h"

#if C_SIMD_SIZE >= 8

#define USE_COMPLEX_CONJ 0
#define CV_SIZE 8
#define DFT_SIZE 4
#define FUNCTION NS(CDftCore_TransposeDftTwiddle_F4_8)

#include "../template/TransposeDftTwiddle_4_v.h"

#endif
