#if CV_SIZE == 1
#include "TransposeDftTwiddle_s_1.h"
#elif CV_SIZE == 2
#include "TransposeDftTwiddle_s_2.h"
#elif CV_SIZE == 4
#include "TransposeDftTwiddle_s_4.h"
#elif CV_SIZE == 8
#include "TransposeDftTwiddle_s_8.h"
#endif
