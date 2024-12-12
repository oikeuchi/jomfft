#pragma once

#include <jomfft/Common.h>

//
// Library settings.
//
#if JOMFFT_USE_SINGLE
#define USE_SINGLE 1
#endif

//
// Basic definitions.
//

#if SIMD_SIZE <= 0

#define R_SIMD_SIZE 2
#define C_SIMD_SIZE 1

#else

#if USE_SINGLE
#define R_SIMD_SIZE (SIMD_SIZE / 4)
#define C_SIMD_SIZE (SIMD_SIZE / 8)
#else
#define R_SIMD_SIZE (SIMD_SIZE / 8)
#define C_SIMD_SIZE (SIMD_SIZE / 16)
#endif

#endif

#if defined(__AVX512F__) && (SIMD_SIZE == 64)
#define R_SIMD_SIZE_S (R_SIMD_SIZE / 2)
#define C_SIMD_SIZE_S (C_SIMD_SIZE / 2)
#else
#define R_SIMD_SIZE_S R_SIMD_SIZE
#define C_SIMD_SIZE_S C_SIMD_SIZE
#endif

#if USE_SINGLE
#define NS(n) jomfftf##n
#define Real float
#else
#define NS(n) jomfft##n
#define Real double
#endif

#define Complex NS(Complex)

#ifdef _MSC_VER
#if USE_SINGLE
#define Complex_(x, y) _FCbuild((Real)(x), (Real)(y))
#define Complex_R(x) _FCbuild((Real)(x), (Real)0)
#else
#define Complex_(x, y) _Cbuild((Real)(x), (Real)(y))
#define Complex_R(x) _Cbuild((Real)(x), (Real)0)
#endif
#else
#define Complex_(x, y) ((Real)(x) + (Real)(y)*I)
#define Complex_R(x) ((Complex)(x))
#endif

#define C_MEMORY_BUS_SIZE (int)(MEMORY_BUS_SIZE / sizeof(Complex))
#define C_L1_CACHE_SIZE (int)(L1_CACHE_SIZE / sizeof(Complex))
#define C_L2_CACHE_SIZE (int)(L2_CACHE_SIZE / sizeof(Complex))
#define C_L3_CACHE_SIZE (int)(L3_CACHE_SIZE / sizeof(Complex))
#define C_MAX_CHUNK_SIZE (int)(MAX_CHUNK_SIZE / sizeof(Complex))
#define C_STRIDE_THRESHOLD (int)(STRIDE_THRESHOLD / sizeof(Complex))
