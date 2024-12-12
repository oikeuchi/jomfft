#pragma once

//
// Standard C include files.
//
#ifdef __GNUC__
#define _GNU_SOURCE
#endif

#include <assert.h>
#include <complex.h>
#include <limits.h>
#include <stdalign.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <math.h>

#ifdef _MSC_VER
#include <malloc.h>
#else
#endif

//
// Check the target architecture.
//
#ifdef _MSC_VER
#if defined(_M_IX64) || defined(_M_X64)
#define CPU_X64
#elif defined(_M_ARM)
#define CPU_ARM
#else
#define CPU_GENERIC
#endif
#else
#if defined(__x86_64__)
#define CPU_X64
#elif defined(__aarch64__)
#define CPU_ARM
#else
#define CPU_GENERIC
#endif
#endif

//
// Architecture-dependent include files.
//
#if !defined(JOMFFT_SIMD_SIZE) || (JOMFFT_SIMD_SIZE > 0)
#ifdef CPU_X64

#include <immintrin.h>
#define SIMD_AVX

#elif defined(CPU_ARM)

#include <arm_neon.h>
#define SIMD_NEON

#else

#if defined(JOMFFT_SIMD_SIZE) && JOMFFT_SIMD_SIZE > 0
#error                                                                         \
    "JOMFFT_SIMD_SIZE is specified but SIMD is not supported in this architecture."
#endif

#endif
#endif

#ifdef _OPENMP
#include <omp.h>
#endif

//
// The header file of this library.
//
#include <jomfft.h>

//
// Macros for Compiliation.
//
#ifdef _MSC_VER
#define FORCEINLINE __forceinline
#else
#define FORCEINLINE __attribute((__always_inline__)) inline
#endif

#ifdef _MSC_VER
#ifdef JOMFFT_DLLEXPORT
#define JOMFFT_API __declspec(dllexport)
#else
#define JOMFFT_API
#endif
#else
#define JOMFFT_API __attribute__((visibility("default")))
#endif

//
// Library settings.
//
#if JOMFFT_PRINT_ERROR
#define PRINT_ERROR 1
#endif

#if JOMFFT_PRINT_MODEL
#define PRINT_MODEL 1
#endif

#ifdef _OPENMP
#define USE_OPENMP 1
#endif

//
// Basic definitions.
//
#ifndef JOMFFT_SIMD_SIZE

#ifdef CPU_X64
#ifdef __AVX512F__
#define JOMFFT_SIMD_SIZE 64
#elif __AVX2__
#define JOMFFT_SIMD_SIZE 32
#else
#define JOMFFT_SIMD_SIZE 16
#endif
#elif defined(CPU_ARM)
#define JOMFFT_SIMD_SIZE 16
#else
#define JOMFFT_SIMD_SIZE 0
#endif

#endif

#define SIMD_SIZE JOMFFT_SIMD_SIZE

#if SIMD_SIZE <= 0
#define ALIGNMENT 16
#else
#define ALIGNMENT SIMD_SIZE
#endif

#define Long long

#include "Memory.h"
#include "Misc.h"
#include "Settings.h"
