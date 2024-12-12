#pragma once

// Memory bus size.
//
// This value is mostly used to determine column size
// and tile size of matrix transposition.
#define MEMORY_BUS_SIZE 64

// L1 cache size.
//
// This value limits the size of pair DFT splitting.
#ifdef JOMFFT_L1_CACHE_SIZE
#define L1_CACHE_SIZE JOMFFT_L1_CACHE_SIZE
#else
#define L1_CACHE_SIZE (1 << 16)
#endif

// L2 cache size.
//
// This value limits the buffer size of matrix transposition,
// especially when the stride is a multiple of STRIDE_THRESHOLD
// below. The cache is assumed to be shared by all cores
// and is divided by num_threads when multithreaded.
#ifdef JOMFFT_L2_CACHE_SIZE
#define L2_CACHE_SIZE JOMFFT_L2_CACHE_SIZE
#else
#define L2_CACHE_SIZE (1 << 19)
#endif

// L3 cache size.
//
// This value limits the buffer size of matrix transposition,
// especially when the stride is a multiple of STRIDE_THRESHOLD
// below. The cache is assumed to be shared by all cores
// and is divided by num_threads when multithreaded.
#ifdef JOMFFT_L3_CACHE_SIZE
#define L3_CACHE_SIZE JOMFFT_L3_CACHE_SIZE
#else
#define L3_CACHE_SIZE (1 << 26)
#endif

// Stride Threshold.
//
// This library switches to some cache aware algorithm
// when the stride * sizeof(VALUE_TYPE) is a multiple of
// this value.
//
// In most of CPUs, a simple nested loop algorithm like
// for (int i = 0; i < m; ++i)
//   for (int j = 0; i < n; ++j)
//     y[y_stride * j + i] = x[x_stride * i + j];
// dramatically slows down when the stride is a multiple
// of 2^k for some k because the address at stride * i (or j)
// collides in the cache for each i (or j).
#define STRIDE_THRESHOLD (1 << 12)

// Stack buffer max size.
//
// This value limits the size of variable length array (VLA)
// based buffering.
#define STACK_BUFFER_MAX_SIZE (1 << 15)

// The minimum DFT size of Bluestein's algorithm.
//
// This value is the minimum dft size for which Bluestein's
// algorithm is applied instead of Singleton's alogorithm
// when the size cannot be factored by small primes <= 7.
#define DFT_BLUESTEIN_MIN_SIZE 128

#if USE_OPENMP

// The minimum DFT size per thread.
//
// This value is to indicate the number of threads
// to be assigned to a single DFT, given by:
//   num_threads = (DFT size) / DFT_MIN_SIZE_PER_THREAD.
// This calculated value is compared to batch size of
// the problem to determine whether dispatching threads
// at the inner loop or the outer loop.
#define DFT_MIN_SIZE_PER_THREAD (1 << 13)

// The prefered DFT size per thread.
//
// This value is to indicate that the problem should
// be multithreaded when
//   (DFT size) >= DFT_PREFERED_SIZE_PER_THREAD.
#define DFT_PREFERED_SIZE_PER_THREAD (1 << 16)

#endif
