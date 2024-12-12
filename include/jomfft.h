#ifndef JOMFFT_H
#define JOMFFT_H

#include <complex.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  jomfftSuccess = 0,
  jomfftInvalidArgument = 1,
  jomfftUnknownError = 255
} jomfftStatus;

typedef enum { jomfftInplace = 0, jomfftNotInplace = 1 } jomfftPlacement;

#ifdef _MSC_VER
#define jomfftComplex _Dcomplex
#define jomfftfComplex _Fcomplex
#else
#define jomfftComplex double complex
#define jomfftfComplex float complex
#endif

struct jomfftComplexDftHandle_;

typedef struct jomfftComplexDftHandle_ *jomfftComplexDftHandle;

struct jomfftfComplexDftHandle_;

typedef struct jomfftfComplexDftHandle_ *jomfftfComplexDftHandle;

struct jomfftRealDftHandle_;

typedef struct jomfftRealDftHandle_ *jomfftRealDftHandle;

struct jomfftfRealDftHandle_;

typedef struct jomfftfRealDftHandle_ *jomfftfRealDftHandle;

#if defined(_MSC_VER) && defined(JOMFFT_DLLEXPORT)
#define JOMFFT_API __declspec(dllexport)
#else
#define JOMFFT_API
#endif

JOMFFT_API int jomfftGetVersionMajor();

JOMFFT_API int jomfftGetVersionMinor();

JOMFFT_API int jomfftGetMaxThreads();

JOMFFT_API jomfftStatus jomfftSetMaxThreads(int num_threads);

JOMFFT_API jomfftStatus jomfftComplexDft(jomfftComplexDftHandle *handle,
                                         int dimension, const int *dft_sizes,
                                         jomfftPlacement placement);

JOMFFT_API jomfftStatus jomfftComplexDftBatch(
    jomfftComplexDftHandle *handle, int dimension, const int *dft_sizes,
    int batch_size, const int *in_storage_sizes, int in_stride, int in_distance,
    const int *out_storage_sizes, int out_stride, int out_distance,
    jomfftPlacement placement);

JOMFFT_API jomfftStatus jomfftComplexDftBatch_(
    jomfftComplexDftHandle *handle, long dimension, const long *dft_sizes,
    long batch_size, const long *in_strides, long in_distance,
    const long *out_strides, long out_distance, jomfftPlacement placement);

JOMFFT_API jomfftStatus jomfftComplexDftForward(jomfftComplexDftHandle handle,
                                                jomfftComplex *x,
                                                jomfftComplex *y);

JOMFFT_API jomfftStatus jomfftComplexDftBackward(jomfftComplexDftHandle handle,
                                                 jomfftComplex *x,
                                                 jomfftComplex *y);

JOMFFT_API jomfftStatus jomfftComplexDftDestruct(jomfftComplexDftHandle handle);

JOMFFT_API jomfftStatus jomfftRealDft(jomfftRealDftHandle *handle,
                                      int dimension, const int *dft_sizes,
                                      jomfftPlacement placement);

JOMFFT_API jomfftStatus jomfftRealDftBatch(
    jomfftRealDftHandle *handle, int dimension, const int *dft_sizes,
    int batch_size, const int *in_storage_sizes, int in_stride, int in_distance,
    const int *out_storage_sizes, int out_stride, int out_distance,
    jomfftPlacement placement);

JOMFFT_API jomfftStatus jomfftRealDftBatch_(
    jomfftRealDftHandle *handle, long dimension, const long *dft_sizes,
    long batch_size, const long *in_strides, long in_distance,
    const long *out_strides, long out_distance, jomfftPlacement placement);

JOMFFT_API jomfftStatus jomfftRealDftForward(jomfftRealDftHandle handle,
                                             double *x, jomfftComplex *y);

JOMFFT_API jomfftStatus jomfftRealDftBackward(jomfftRealDftHandle handle,
                                              jomfftComplex *x, double *y);

JOMFFT_API jomfftStatus jomfftRealDftDestruct(jomfftRealDftHandle handle);

JOMFFT_API jomfftStatus jomfftfComplexDft(jomfftfComplexDftHandle *handle,
                                          int dimension, const int *dft_sizes,
                                          jomfftPlacement placement);

JOMFFT_API jomfftStatus jomfftfComplexDftBatch(
    jomfftfComplexDftHandle *handle, int dimension, const int *dft_sizes,
    int batch_size, const int *in_storage_sizes, int in_stride, int in_distance,
    const int *out_storage_sizes, int out_stride, int out_distance,
    jomfftPlacement placement);

JOMFFT_API jomfftStatus jomfftfComplexDftBatch_(
    jomfftfComplexDftHandle *handle, long dimension, const long *dft_sizes,
    long batch_size, const long *in_strides, long in_distance,
    const long *out_strides, long out_distance, jomfftPlacement placement);

JOMFFT_API jomfftStatus jomfftfComplexDftForward(jomfftfComplexDftHandle handle,
                                                 jomfftfComplex *x,
                                                 jomfftfComplex *y);

JOMFFT_API jomfftStatus jomfftfComplexDftBackward(
    jomfftfComplexDftHandle handle, jomfftfComplex *x, jomfftfComplex *y);

JOMFFT_API jomfftStatus
jomfftfComplexDftDestruct(jomfftfComplexDftHandle handle);

JOMFFT_API jomfftStatus jomfftfRealDft(jomfftfRealDftHandle *handle,
                                       int dimension, const int *dft_sizes,
                                       jomfftPlacement placement);

JOMFFT_API jomfftStatus jomfftfRealDftBatch(
    jomfftfRealDftHandle *handle, int dimension, const int *dft_sizes,
    int batch_size, const int *in_storage_sizes, int in_stride, int in_distance,
    const int *out_storage_sizes, int out_stride, int out_distance,
    jomfftPlacement placement);

JOMFFT_API jomfftStatus jomfftfRealDftBatch_(
    jomfftfRealDftHandle *handle, long dimension, const long *dft_sizes,
    long batch_size, const long *in_strides, long in_distance,
    const long *out_strides, long out_distance, jomfftPlacement placement);

JOMFFT_API jomfftStatus jomfftfRealDftForward(jomfftfRealDftHandle handle,
                                              float *x, jomfftfComplex *y);

JOMFFT_API jomfftStatus jomfftfRealDftBackward(jomfftfRealDftHandle handle,
                                               jomfftfComplex *x, float *y);

JOMFFT_API jomfftStatus jomfftfRealDftDestruct(jomfftfRealDftHandle handle);

#undef JOMFFT_API

#ifdef __cplusplus
}
#endif

#endif
