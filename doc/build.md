# Building and Installing.

## Prerequisites.
* CMake version >= 3.21.
* A GCC compatible compiler supporting standard C11.
* A MSVC without standard C11 support also works (but is not clear from which version this project can be compiled).
* (Optional for threading) Open MP version >= 4.0.

As of 2024, MSVCs support Open MP 4.0 through their LLVM Open MP module. It requires CMake 3.30 to configure MSVCs to link this module.

## SIMD support.
* AVX2 and AVX512 in x64.
* Neon in Arm.

If no supported SIMD instructions are available, it falls back to basic arithmetic of C99 complex types (though not seriously optimized).

## Building.
To build and install this project, just run cmake and make.
```console
$ cmake -DCMAKE_BUILD_TYPE=Release <path-to-jomfft-repo>
$ make
$ make install
```
If no options are specified, make will generate a static library.

## Testing.
To run basic tests of this library, please specify 'JOMFFT\_TEST=ON' to cmake.
```console
$ cmake -DCMAKE_BUILD_TYPE=Debug -DJOMFFT_TEST=ON <path-to-jomfft-repo>
$ make
$ ./jomfft_test
```
There is also a test executable for single precision but it may sometimes fail due to numerical errors.

## Options for CMakeLists.txt.

*  JOMFFT\_LIB\_STATIC (ON/OFF) This option controls whether to generate a static library. The default value is ON.

*  JOMFFT\_LIB\_SHARED (ON/OFF) This option controls whether to generate a shared library (or a DLL in Windows). The default value is OFF.

*  JOMFFT\_SIMD\_SIZE (Integer) This option represents the size of SIMD vector in bytes. For a GCC compatible compiler, this library automatically determines the largest possible size by setting '-march=native'. For a MSVC, its default value is 32 for AMD64 and is 16 for ARM. If non-positive value is specified, no SIMD instructions are used in compilation. Positive values other than 16, 32 and 64 may result compile errors.

*  JOMFFT\_ENABLE\_OPENMP (ON/OFF) This option enables Open MP threading. The default value is OFF. For MSVCs, (as of 2024) a LLVM version of Open MP modules will be used to support Open MP 4.0 (for collapse directive of nested loops).

*  JOMFFT\_PRINT\_ERROR (ON/OFF) This option enables error messages in stderr. Mostly, it reports invalid argument errors in APIs. The default value is OFF.

*  JOMFFT\_DISABLE\_DOUBLE (ON/OFF) This option disables double precision APIs. The default value is OFF.

*  JOMFFT\_DISABLE\_SINGLE (ON/OFF) This option disables single precision APIs. The default value is OFF.

*  JOMFFT\_TEST (ON/OFF) This option controls whether to generate test executable. The default value is OFF. If some APIs are disabled by specifying JOMFFT\_DISABLE\_DOUBLE or JOMFFT\_DISABLE\_SINGLE, a test executable for disabled APIs will not be generated.
