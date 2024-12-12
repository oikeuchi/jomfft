#include "TransposeDftTwiddle_F2.h"

#if C_SIMD_SIZE >= 4

#define USE_COMPLEX_CONJ 0
#define CV_SIZE 4
#define DFT_SIZE 2
#define FUNCTION NS(CDftCore_TransposeDftTwiddle_F2_4)

#include "../template/TransposeDftTwiddle_2_v.h"

#endif
