# Jomfft

Jomfft is a simple yet powerful open-source FFT library written in C. Its performance is comparable to highly optimized libraries such as FFTW3 or Intel MKL. Multithreading through Open MP is also supported.

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
double complex *x = DataToTransform();
// Create a handle for complex in-place DFT.
int dft_sizes[] = {64, 64};
jomfftHandle handle = 0;
jomfftComplexDft(&handle, 2, dft_sizes, jomfftInplace);
// Perform forward transform.
jomfftComplexDftForward(&handle, x, x);
// Do some processing.
DoSomeProcessing(x);
// Perform backward (conjugate) transform.
jomfftComplexDftBackward(&handle, x, x);
```

```C
// Prepare a real input data.
double *x = DataToTransform();
// Create a handle for real in-place DFT.
int dft_sizes[] = {64, 64};
jomfftHandle handle = 0;
jomfftRealDft(&handle, 2, dft_sizes, jomfftInplace);
// Reinterpret x as a complex conjugate-even output.
// In this case, x is assumed to be a double array of size 64x66 and
// y is assumed to be a double complex array of size 64x33.
double complex *y = (double complex *)x;
// Perform forward transform.
jomfftRealDftForward(&handle, x, y);
// Do some processing.
DoSomeProcessing(y);
// Perform backward (conjugate) transform.
jomfftRealDftBackward(&handle, y, x);
```

Batch processing and user-defined storage shapes are also supported. Please refer the 'API' section of Wiki.

# Performance

Here are performance comparisons with FFTW3 and Intel MKL on AMD Ryzen 5900X (xxx GHz with AVX2) of randomly generated double precision 2D in-place real DFTs.

* Single thread.

|Size|Jomfft|FFTW3|Intel MKL|
|---|---|---|---|---|
|64x64|... sec|... sec|... sec|

* 8 threads with Open MP.

|Size|Jomfft|FFTW3|Intel MKL|
|---|---|---|---|---|
|64x64|... sec|... sec|... sec|

For further comparison, please refer the 'Performance Comparision' section of Wiki.

# Lincense

Jomfft is distributed under the MIT License.

# About Naming of This Library

Jomfft is named after a repeatedly used phrase in a classic American crime drama TV series 'Columbo'.
