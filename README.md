# Jomfft

Jomfft is a simple yet powerful open-source FFT library written in C. Its performance is comparable to highly optimized libraries such as FFTW3 and/or Intel MKL. Multithreading through Open MP is also supported.

# Building and Installing

Jomfft is written in standard C11 and uses CMake for its build system.

## Prerequisites
* CMake version >= 3.21.
* A GCC compatible compiler supporting standard C11.
* A MSVC without standard C11 support also works (but is not clear from which version this project can be compiled).
* (Optional for threading) Open MP version >= 4.0.

```console
$ cmake -DCMAKE_BUILD_TYPE=Release <path-to-jomfft-repo>
$ make
$ make install
```

For build options, please refer the 'Build' secition of Wiki.

# Usage

Jomfft has a clean C interface. Here are some examples.

```C
// Prepare a complex input data.
double complex *x = ...;
// Create a handle for complex in-place DFT.
int dft_sizes[] = {64, 64};
jomfftHandle handle = 0;
jomfftComplexDft(&handle, 2, dft_sizes, jomfftInplace);
// Perform forward transform.
jomfftComplexDftForward(handle, x, x);
// Do some processing.
...
// Perform backward (conjugate) transform.
jomfftComplexDftBackward(handle, x, x);
```

```C
// Prepare a real input data.
double *x = ...;
// Create a handle for real in-place DFT.
int dft_sizes[] = {64, 64};
jomfftHandle handle = 0;
jomfftRealDft(&handle, 2, dft_sizes, jomfftInplace);
// Reinterpret x as a complex conjugate-even output.
// In this case, x is assumed to be a double array of size 64x66 and
// y is assumed to be a double complex array of size 64x33.
double complex *y = (double complex *)x;
// Perform forward transform.
jomfftRealDftForward(handle, x, y);
// Do some processing.
...
// Perform backward (conjugate) transform.
jomfftRealDftBackward(handle, y, x);
```

Batch processing and user-defined storage shapes are also supported. Please refer the 'API' section of Wiki.

# Performance

Here are performance comparisons with FFTW3 and Intel MKL on AMD Ryzen 5900X (xxx GHz with AVX2) of randomly generated double precision 2D in-place real DFTs.

## Double Precision, Single Thread (Elapsed Seconds).

|Size|Jomfft|FFTW3|MKL|
|---:|---:|---:|---:|
|64x64|9.3910e-06|9.3304e-06|1.7783e-05|
|256x256|2.6751e-04|2.5858e-04|4.2263e-04|
|1024x1024|5.4148e-03|5.3602e-03|9.6216e-03|
|4096x4096|1.4980e-01|1.4699e-01|7.1150e-01|

## Double Precision, 8 Threads (Elapsed Seconds).

|Size|Jomfft|FFTW3|MKL|
|---:|---:|---:|---:|
|256x256|5.8219e-05|6.6876e-05|8.4924e-05|
|1024x1024|1.4694e-03|1.1021e-03|1.3188e-03|
|4096x4096|5.0559e-02|4.3677e-02|5.4363e-02|

For further comparison, please refer the 'Performance Comparision' section of Wiki.

# Lincense

Jomfft is distributed under the MIT License.

# About Naming of This Library

Jomfft is named after a repeatedly used phrase in a classic American crime drama TV series 'Columbo'.
