#include "TransposeDftTwiddle_B4.h"

#define USE_COMPLEX_CONJ 1
#define CV_SIZE MIN(C_SIMD_SIZE, 4)
#define DFT_SIZE 4
#define FUNCTION NS(CDftCore_TransposeDftTwiddle_B4)
#define TEMPLATE_FILE "Dft_4.h"

#include "../template/TransposeDftTwiddle.h"
