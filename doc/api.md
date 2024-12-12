# API Reference.

This is a reference manual of Jomfft APIs. Jomfft has APIs for both single and double precision. Jomfft's single precision APIs are prefixed by 'jomfftf' whereas its double precision APIs are by 'jomfft', similarly to FFTW.

* An instantiated DFT handle possesses precomputed constants (twiddle factors) and should be destructed by an appropriate "jomfftXXXDesctruct" function when a handle is no longer used.

* Jomfft has special APIs for real data and conjugate-even data. They are prefixed by jomfftRealDftXXX. jomfftRealDftForward computes real to conjugate-even DFTs whereas jomfftRealDftBackward computes conjugate-even to real DFTs.

* All the forward and backward transforms within the same instance share their precomputed constants and its overhead is minimum if users call only one of forward or backward transform. For simplicity (especially to avoid introducing reference counting containers), precomputed constants are not shared among different handles.

* All DFT APIs are thread-safe but destructing a handle during DFT computation with the same handle in other threads may lead to undefined behavior.

* When Open MP is enabled in build, DFT computation is threaded through Open MP. The maximum number of threads can be controlled by calling jomfftSetMaxThreads or by directly limiting the maximum number of threads of a Open MP library linked to Jomfft.

## Types and Enums.

### jomfftComplex / jomfftfComplex
Complex types used in this library. For MSVCs, their special complex types (\_Dcomplex/\_Fcomplex) are used since it is not easy to setup C99 complex types properly. Otherwise, C99 complex types are used.

### jomfftComplexDftHandle / jomfftfComplexDftHandle
A handle for complex DFT APIs.

### jomfftComplexDftHandle / jomfftfComplexDftHandle
A handle for real DFT APIs.

### jomfftStatus
An enum for reporting API results.
* jomfftSuccess: This value is to notify a API call is succeeded.
* jomfftInvalidArgument: This value is to notify a API call has some invalid argument. This library outputs error messages if JOMFFT\_PRINT\_ERROR compile option is specified.
* jomfftUnknownError: This value is to notify a API failed with unknown errors.

### jomfftPlacement
An enum for specifying data placement in DFT computation.
* jomfftInplace: This value is to specify that computation should be performed in-place.
* jomfftNotInplace: This value is to specify that computation should be performed not-in-place.

## System Functions.

### jomfftGetVersionMajor
```C
int jomfftGetVersionMajor();
```
This function returns the major version of this library.

### jomfftGetVersionMinor
```C
int jomfftGetVersionMinor();
```
This function returns the minor version of this library.

### jomfftGetMaxThreads
```C
int jomfftGetMaxThreads();
```
This function returns the maximum number of threads used in this library. When Open MP is enabled and the maximum number is not set by jomfftSetMaxThreads or non-positive number is set, this function returns the same value as of omp\_get\_max\_threads().

### jomfftSetMaxThreads
```C
jomfftStatus jomfftSetMaxThreads(int num_threads);
```
This function sets the maximum number of threads used in this library. When a non-positive number is specified, the return value of omp\_get\_max\_threads will be used instead. This function is not thread-safe.

* num\_threads: The maximum number of threads.

## Complex DFT Functions.

### jomfftComplexDft / jomfftfComplexDft
```C
jomfftStatus jomfftComplexDft(jomfftComplexDftHandle *handle,
                              int dimension,
                              const int *dft_sizes, 
                              jomfftPlacement placement);
```
This function create a new instance that computes a complex DFT of specified sizes. If users need to specify storage sizes different from dft\_sizes, please use the batch API below.

* handle: A pointer of an output handle. When error occurred, its value is not modified.
* dimension: Dimension of DFT.
* dft\_sizes: A size array of DFTs with length of dimension.
* placement: Placement in DFT computation.
* Return value: jomfftSuccess is returned when successfully instantiated. Otherwise, jomfftInvalidArgument is returned.

Example:
```C
int dft_sizes[] = {256, 256};
jomfftComplexDftHandle handle = 0;
jomfftComplexDft(&handle, 2, dft_sizes, jomfftInplace);
```

### jomfftComplexDftBatch / jomfftfComplexDftBatch
```C
jomfftStatus jomfftComplexDftBatch(jomfftComplexDftHandle *handle,
                                   int dimension,
                                   const int *dft_sizes,
                                   int batch_size,
                                   const int *in_storage_sizes, 
                                   int in_stride,
                                   int in_distance,
                                   const int *out_storage_sizes,
                                   int out_stride,
                                   int out_distance,
                                   jomfftPlacement placement);
```
This function create a new instance that computes a batch of complex DFTs of specified sizes. 

* handle: A pointer of an output handle. When error occurred, its value is not modified.
* dimension: Dimension of DFT.
* dft\_sizes: A size array of DFTs with length of dimension.
* batch\_size: Batch size of DFT.
* in\_storage\_sizes: A size array of input data.
* in\_stride: Input data stride of batch processing.
* in\_distance: Input data distance of batch processing.
* out\_storage\_sizes: A size array of output data.
* out\_stride: Output data stride of batch processing.
* out\_distance: Output data distance of batch processing.
* placement: Placement in DFT computation.
* Return value: jomfftSuccess is returned when successfully instantiated. Otherwise, jomfftInvalidArgument is returned.

The argument of input / output storage\_sizes is to specify the layout of input / output data. For example, when dft\_sizes are 16x12, input data is on a 16x12 array and output data is on a 16x16 array with 4 elements padded on every row, API call will be something like this.
```C
int dft_sizes[] = {16, 12};
int in_storage_sizes[] = {16, 12};
int out_storage_sizes[] = {16, 16};
jomfftComplexDftHandle handle = 0;
jomfftComplexDft(&handle, 2, dft_sizes, 1, in_storage_sizes, 1, 0, 
                 out_storage_sizes, 1, 0, jomfftNotInplace);
```

The arguments of input / output stride and distance are to specify how a API treats input / output data in its batch loop. It is processed as in the following pseudo code.
```C
for (int i = 0; i < batch_size; ++i) {
  // Run DFT algorithms with the specified input stride and the output stride.
  RunDftOnStridedData(&input_data[in_distance * i], in_stride,
                      &output_data[out_distance * i], out_stride);
}
```
In actual computation, some optimized routines are invoked when stride == 1 or distance == 1. Usually, a batch on consecutive data (in\_stride == 1 and out \_stride == 1) is the fastest among stride / distance variations.

### jomfftComplexDftBatch_ / jomfftfComplexDftBatch_
```C
jomfftStatus jomfftComplexDftBatch_(jomfftComplexDftHandle *handle,
                                    long dimension,
                                    const long *dft_sizes,
                                    long batch_size,
                                    const long *in_strides,
                                    long in_distance,
                                    const long *out_strides,
                                    long out_distance, 
                                    jomfftPlacement placement);
```
This function is to provide an API for internal instantiating functions which take long-valued sizes and strides. For most cases, the above two APIs without underscore are recommended.

* handle: A pointer of an output handle. When error occurred, its value is not modified.
* dimension: Dimension of DFT.
* dft\_sizes: A size array of DFTs with length of dimension.
* batch\_size: Batch size of DFT.
* in\_strides: A stride array of input data.
* in\_distance: Input data distance of batch processing.
* out\_strides: A stride array of output data.
* out\_distance: Output data distance of batch processing.
* placement: Placement in DFT computation.
* Return value: jomfftSuccess is returned when successfully instantiated. Otherwise, jomfftInvalidArgument is returned.

### jomfftComplexDftForward / jomfftfComplexDftForward 
```C
jomfftStatus jomfftComplexDftForward(jomfftComplexDftHandle handle,
                                     jomfftComplex *x,
                                     jomfftComplex *y);
```
This function computes a forward (non-conjugate) DFT.

* handle: A handle.
* x: A pointer to input data.
* y: A pointer to output data.

### jomfftComplexDftBackward / jomfftfComplexDftBackward 
```C
jomfftStatus jomfftComplexDftBackward(jomfftComplexDftHandle handle,
                                      jomfftComplex *x,
                                      jomfftComplex *y);
```
This function computes a backward (conjugate) DFT. In this function, the output data layout specified in instantiating function is used for input and the input data layout is used for output.

* handle: A handle.
* x: A pointer to input data.
* y: A pointer to output data.


### jomfftComplexDftDestruct / jomfftfComplexDftDestruct 
```C
jomfftStatus jomfftComplexDftDestruct(jomfftComplexDftHandle handle);
```
This function destructs an instance pointed by the argument.

## Real DFT Functions.

Jomfft's APIs for real DFTs compute a real to conjugate-even transform and its backward. The storage scheme of one-dimensional conjugate-even data consists of the first $\lfloor N / 2 \rfloor + 1$ elements of a complex vector where $N$ is a size of the conjugate-even vector as a complex vector. When $N$ is a multiple of two, this scheme requires extra storage for one complex value compared to its real counterpart ($2 (N/2 + 1) - N = 2$ in real values). In multidimensional cases, the innermost axis (in row-major order) is always assumed to be in this one-dimensional conjugate-even storage scheme.

In this scheme, computation on axes other than the innermost axis can be performed by simply applying complex DFT algorithms and somewhat complicated real to conjugate-even (or its backward) transform can be performed on consecutive data. (Strided data can also be treated by this library but is less efficient in computation.) In fact, this slightly redundant scheme is (arguably) the most popular one and adopted in many FFT libraries including FFTW3. 

While not available in this library, there are also some packed schemes to save this extra storage for one complex value. For interested readers, please refer the documentation of Intel MKL.


### jomfftRealDft / jomfftfRealDft
```C
jomfftStatus jomfftRealDft(jomfftRealDftHandle *handle,
                           int dimension,
                           const int *dft_sizes, 
                           jomfftPlacement placement);
```
This function create a new instance that computes a complex DFT of specified sizes. If users need to specify storage sizes different from dft\_sizes, please use the batch API below.

* handle: A pointer of an output handle. When error occurred, its value is not modified.
* dimension: Dimension of DFT.
* dft\_sizes: A size array of DFTs with length of dimension.
* placement: Placement in DFT computation.
* Return value: jomfftSuccess is returned when successfully instantiated. Otherwise, jomfftInvalidArgument is returned.

When jomfftInplace is specified, the input storage sizes are assumed to be 
$$
\text{in\_storage\_sizes}[i] = \begin{cases} 2 (\lfloor \text{dft\_sizes}[i] / 2 \rfloor  + 1) & \text{if } i = \text{dimension} - 1, \\
        \text{dft\_sizes}[i] & \text{otherwise}. \\
        \end{cases}
$$
When jomfftNotInplace is specified, the input storage sizes are assumed to be the same as dft\_sizes.
In both cased the output storage sizes are assumed to be
$$
\text{out\_storage\_sizes}[i] = \begin{cases} (\lfloor \text{dft\_sizes}[i] / 2 \rfloor  + 1) & \text{if } i = \text{dimension} - 1, \\
        \text{dft\_sizes}[i] & \text{otherwise}. \\
        \end{cases}
$$

Examples: In-place computation.
```C
int dft_sizes[] = {256, 256};
int in_storage_sizes[] = {256, 258}; // 2 (256 / 2 + 1) = 258.
int out_storage_sizes[] = {256, 129}; // 256 / 2 + 1 = 129.
jomfftRealDftHandle handle = 0;
jomfftRealDft(&handle, 2, dft_sizes, jomfftInplace);
double *x = (double *)malloc(sizeof(double) * 
        in_storage_sizes[0] * in_storage_sizes[1]);
double complex *y = (double complex *)x;
DoInitializeInput(x); // Do some user-defined initialization.
jomfftRealDftForward(handle, x, y);
DoSomeProcessing(y); // Do some user-defined processing.
jomfftRealDftBackward(handle, y, x);
jomfftRealDftDestruct(handle);
DoReportResult(x); // Do some user-defined reporting.
free(x);
```

Not-in-place computation.
```C
int dft_sizes[] = {256, 256};
int in_storage_sizes[] = {256, 256};
int out_storage_sizes[] = {256, 129}; // 256 / 2 + 1 = 129.
jomfftRealDftHandle handle = 0;
jomfftRealDft(&handle, 2, dft_sizes, jomfftNotInplace);
double *x = (double *)malloc(sizeof(double) * 
        in_storage_sizes[0] * in_storage_sizes[1]);
double complex *y = (double complex *)malloc(sizeof(double complex) * 
        out_storage_sizes[0] * out_storage_sizes[1]);
DoInitializeInput(x); // Do some user-defined initialization.
jomfftRealDftForward(handle, x, y);
DoSomeProcessing(y); // Do some user-defined processing.
jomfftRealDftBackward(handle, y, x);
jomfftRealDftDestruct(handle);
DoReportResult(x); // Do some user-defined reporting.
free(x);
free(y);
```

### jomfftRealDftBatch / jomfftfRealDftBatch
```C
jomfftStatus jomfftRealDftBatch(jomfftRealDftHandle *handle,
                                int dimension,
                                const int *dft_sizes,
                                int batch_size,
                                const int *in_storage_sizes, 
                                int in_stride,
                                int in_distance,
                                const int *out_storage_sizes,
                                int out_stride,
                                int out_distance,
                                jomfftPlacement placement);
```
This function create a new instance that computes a batch of complex DFTs of specified sizes. 

* handle: A pointer of an output handle. When error occurred, its value is not modified.
* dimension: Dimension of DFT.
* dft\_sizes: A size array of DFTs with length of dimension.
* batch\_size: Batch size of DFT.
* in\_storage\_sizes: A size array of input data.
* in\_stride: Input data stride of batch processing.
* in\_distance: Input data distance of batch processing.
* out\_storage\_sizes: A size array of output data.
* out\_stride: Output data stride of batch processing.
* out\_distance: Output data distance of batch processing.
* placement: Placement in DFT computation.
* Return value: jomfftSuccess is returned when successfully instantiated. Otherwise, jomfftInvalidArgument is returned.

The argument of input / output storage\_sizes is to specify the layout of input / output data. For example, when dft\_sizes are 16x12, input data is on a 16x12 array and output data is on a 16x8 array with 4 elements padded on every row, API call will be something like this.
```C
int dft_sizes[] = {16, 12};
int in_storage_sizes[] = {16, 12};
int out_storage_sizes[] = {16, 8};
jomfftRealDftHandle handle = 0;
jomfftRealDft(&handle, 2, dft_sizes, 1, in_storage_sizes, 1, 0, 
                 out_storage_sizes, 1, 0, jomfftNotInplace);
```

The arguments of input / output stride and distance are to specify how a API treats input / output data in its batch loop. It is processed as in the following pseudo code.
```C
for (int i = 0; i < batch_size; ++i) {
  // Run DFT algorithms with the specified input stride and the output stride.
  RunDftOnStridedData(&input_data[in_distance * i], in_stride,
                      &output_data[out_distance * i], out_stride);
}
```
In actual computation, some optimized routines are invoked when stride == 1 or distance == 1. Usually, a batch on consecutive data (in\_stride == 1 and out \_stride == 1) is the fastest among stride / distance variations.

### jomfftRealDftBatch_ / jomfftfRealDftBatch_
```C
jomfftStatus jomfftRealDftBatch_(jomfftRealDftHandle *handle,
                                    long dimension,
                                    const long *dft_sizes,
                                    long batch_size,
                                    const long *in_strides,
                                    long in_distance,
                                    const long *out_strides,
                                    long out_distance, 
                                    jomfftPlacement placement);
```
This function is to provide an API for internal instantiating functions which take long-valued sizes and strides. For most cases, the above two APIs without underscore are recommended.

* handle: A pointer of an output handle. When error occurred, its value is not modified.
* dimension: Dimension of DFT.
* dft\_sizes: A size array of DFTs with length of dimension.
* batch\_size: Batch size of DFT.
* in\_strides: A stride array of input data.
* in\_distance: Input data distance of batch processing.
* out\_strides: A stride array of output data.
* out\_distance: Output data distance of batch processing.
* placement: Placement in DFT computation.
* Return value: jomfftSuccess is returned when successfully instantiated. Otherwise, jomfftInvalidArgument is returned.

### jomfftRealDftForward / jomfftfRealDftForward 
```C
jomfftStatus jomfftRealDftForward(jomfftRealDftHandle handle,
                                  double *x,
                                  double *y);
```
This function computes a forward (real to conjugate-even) DFT.

* handle: A handle.
* x: A pointer to input data.
* y: A pointer to output data.

### jomfftRealDftBackward / jomfftfRealDftBackward 
```C
jomfftStatus jomfftRealDftBackward(jomfftRealDftHandle handle,
                                   double *x,
                                   double *y);
```
This function computes a backward (conjugate-even to real) DFT. In this function, the output data layout specified in instantiating function is used for input and the input data layout is used for output.

* handle: A handle.
* x: A pointer to input data.
* y: A pointer to output data.


### jomfftRealDftDestruct / jomfftfRealDftDestruct 
```C
jomfftStatus jomfftRealDftDestruct(jomfftRealDftHandle handle);
```
This function destructs an instance pointed by the argument.

* handle: A handle.
