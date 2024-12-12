#include "TransposeDftTwiddle_B2.h"

#define USE_COMPLEX_CONJ 1
#define CV_SIZE MIN(C_SIMD_SIZE, 2)
#define DFT_SIZE 2
#define FUNCTION NS(CDftCore_TransposeDftTwiddle_B2)
#define TEMPLATE_FILE "Dft_2.h"

#include "../template/TransposeDftTwiddle.h"
