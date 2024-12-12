cmake_minimum_required(VERSION 3.21)

# CMakeLists.txt for Jomfft.
#
# Options for libraries:
#   JOMFFT_LIB_STATIC: ON/OFF: Output a static library (default: ON).
#   JOMFFT_LIB_SHARED: ON/OFF: Output a shared library (default: OFF).
#   JOMFFT_SIMD_SIZE: Integer: SIMD vector size in bytes 
#     (default: GCC: max size for native CPUs / MSVC: 32 for AMD64, 16 for ARM).
#   JOMFFT_ENABLE_OPENMP: ON/OFF: Enable Open MP (default: OFF).
#   JOMFFT_PRINT_ERROR: ON/OFF: Enable printing errors to stderr (default: OFF).
#   JOMFFT_DISABLE_DOUBLE: ON/OFF: Disable double precision APIs (default: OFF).
#   JOMFFT_DISABLE_SINGLE: ON/OFF: Disable single precision APIs (default: OFF).
#   JOMFFT_L1_CACHE_SIZE: Integer: L1 cache size in bytes (default: 64KiB).
#   JOMFFT_L2_CACHE_SIZE: Integer: L2 cache size in bytes (default: 512KiB).
#   JOMFFT_L3_CACHE_SIZE: Integer: L3 cache size in bytes (default: 64MiB).
#
# Options for tests and others:
#   JOMFFT_TEST: ON/OFF: Output test executable (default: OFF).
#   JOMFFT_LIB_TEST: ON/OFF: Output executable for library link test (default: OFF).
#   JOMFFT_COMPARE: ON/OFF: Output executable comparing outputs against FFTW3 (default: OFF).
#   JOMFFT_BENCH: ON/OFF: Output executable for benchmark (default: OFF).
#   JOMFFT_BENCH_FFTW: ON/OFF: Enable FFTW3 in benchmark executable (default: OFF).
#   JOMFFT_BENCH_MKL: ON/OFF: Enable MKL in benchmark executable (default: OFF).
#
# Options for developers:
#   JOMFFT_PRINT_MODEL: ON/OFF: Enable printing information of instantiated models to stdout (default: OFF).
#   JOMFFT_CHECK_ADDRESS: ON/OFF: Enable '-fsanitize=address' option of GCC (default: OFF).
#   JOMFFT_PROFILE: ON/OFF: Enable '-pg' option of GCC (default: OFF).

if(NOT DEFINED JOMFFT_LIB_STATIC)
  set(JOMFFT_LIB_STATIC ON)
endif()
if(NOT DEFINED JOMFFT_L1_CACHE_SIZE)
  set(JOMFFT_L1_CACHE_SIZE 65536)
endif()
if(NOT DEFINED JOMFFT_L2_CACHE_SIZE)
  set(JOMFFT_L2_CACHE_SIZE 524288)
endif()
if(NOT DEFINED JOMFFT_L3_CACHE_SIZE)
  set(JOMFFT_L3_CACHE_SIZE 67108864)
endif()

file(STRINGS "VERSION.txt" JOMFFT_VERSION)

project(jomfft VERSION ${JOMFFT_VERSION} LANGUAGES C)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/common/jomfft/Version.h.in
  ${CMAKE_CURRENT_SOURCE_DIR}/common/jomfft/Version.h
  @ONLY)

if(NOT CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  set(CMAKE_C_FLAGS_RELEASE_INIT "-O3 -DNDEBUG")
endif()

if(NOT DEFINED ARCH_FLAGS)
  if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "AMD64") 
      set(ARCH_FLAGS "")
      if(NOT DEFINED JOMFFT_SIMD_SIZE OR JOMFFT_SIMD_SIZE GREATER_EQUAL 32)
        list(APPEND ARCH_FLAGS /arch:AVX2)
      endif()
      if(JOMFFT_SIMD_SIZE EQUAL 64)
        list(APPEND ARCH_FLAGS /arch:AVX512)
      endif()
    endif()
    list(APPEND ARCH_FLAGS /fp:fast)
  else()
    set(ARCH_FLAGS -march=native -mtune=native -ffast-math)
  endif()
endif()

if(JOMFFT_ENABLE_OPENMP)
  if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    cmake_minimum_required(VERSION 3.30)
    set(OpenMP_RUNTIME_MSVC llvm)
  endif()
  find_package(OpenMP REQUIRED)
  list(APPEND ARCH_FLAGS ${OpenMP_C_FLAGS})
endif()

if(NOT DEFINED WARN_FLAGS)
  if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(WARN_FLAGS /W4 /wd4701 /wd4127 -D_CRT_SECURE_NO_WARNINGS)
  else()
    set(WARN_FLAGS -Wall -Wextra)
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
      list(APPEND WARN_FLAGS -Wno-maybe-uninitialized)
    endif()
  endif()
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  if(JOMFFT_LIB_SHARED)
    list(APPEND EXTRA_FLAGS -DJOMFFT_DLLEXPORT)
  endif()
endif()

if(DEFINED JOMFFT_SIMD_SIZE)
  list(APPEND EXTRA_FLAGS -DJOMFFT_SIMD_SIZE=${JOMFFT_SIMD_SIZE})
endif()
if (DEFINED JOMFFT_L1_CACHE_SIZE)
  list(APPEND EXTRA_FLAGS -DJOMFFT_L1_CACHE_SIZE=${JOMFFT_L1_CACHE_SIZE})
endif()
if (DEFINED JOMFFT_L2_CACHE_SIZE)
  list(APPEND EXTRA_FLAGS -DJOMFFT_L2_CACHE_SIZE=${JOMFFT_L2_CACHE_SIZE})
endif()
if (DEFINED JOMFFT_L3_CACHE_SIZE)
  list(APPEND EXTRA_FLAGS -DJOMFFT_L3_CACHE_SIZE=${JOMFFT_L3_CACHE_SIZE})
endif()

if(JOMFFT_PRINT_ERROR)
  list(APPEND EXTRA_FLAGS -DJOMFFT_PRINT_ERROR=1)
endif()

if(JOMFFT_PRINT_MODEL)
  list(APPEND EXTRA_FLAGS -DJOMFFT_PRINT_MODEL=1)
endif()

if(JOMFFT_PROFILE)
  list(APPEND EXTRA_FLAGS -pg)
  list(APPEND LDFLAGS -pg)
endif()

if(JOMFFT_CHECK_ADDRESS)
  if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    list(APPEND EXTRA_FLAGS -fsanitize=address)
    list(APPEND LDFLAGS -fsanitize=address)
  endif()
endif()

set(CFLAGS_ ${ARCH_FLAGS} ${WARN_FLAGS} ${EXTRA_FLAGS})
set(CFLAGS_DOUBLE ${CFLAGS_})
set(CFLAGS_SINGLE ${CFLAGS_} -DJOMFFT_USE_SINGLE=1)

set(PREFIXES "")
if(NOT JOMFFT_DISABLE_DOUBLE)
  list(APPEND PREFIXES jomfft)
endif()
if(NOT JOMFFT_DISABLE_SINGLE)
  list(APPEND PREFIXES jomfftf)
endif()

set(COMMON_OBJLIB "jomfft_common")
add_library(${COMMON_OBJLIB} OBJECT) 
set_property(TARGET ${COMMON_OBJLIB} PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET ${COMMON_OBJLIB} PROPERTY C_STANDARD 11)
set_property(TARGET ${COMMON_OBJLIB} PROPERTY C_VISIBILITY_PRESET hidden)
add_subdirectory(common)
add_subdirectory(include)
target_include_directories(${COMMON_OBJLIB} PRIVATE include)
target_compile_options(${COMMON_OBJLIB} PRIVATE
  $<$<COMPILE_LANGUAGE:C>:>${CFLAGS_})

foreach(P IN LISTS PREFIXES)
  set(OBJLIB "${P}_objlib")
  if(${P} STREQUAL "jomfftf")
    set(CFLAGS ${CFLAGS_SINGLE})
  else()
    set(CFLAGS ${CFLAGS_DOUBLE})
  endif()
  add_library(${OBJLIB} OBJECT) 
  set_property(TARGET ${OBJLIB} PROPERTY POSITION_INDEPENDENT_CODE ON)
  set_property(TARGET ${OBJLIB} PROPERTY C_STANDARD 11)
set_property(TARGET ${OBJLIB} PROPERTY C_VISIBILITY_PRESET hidden)
  add_subdirectory(dft "${CMAKE_CURRENT_BINARY_DIR}/${P}/dft")
  target_include_directories(${OBJLIB} PRIVATE include common)
  target_compile_options(${OBJLIB} PRIVATE
    $<$<COMPILE_LANGUAGE:C>:>${CFLAGS})
endforeach() 

if(JOMFFT_LIB_STATIC)
  set(LIB_STATIC "jomfft_static")
  add_library(${LIB_STATIC} STATIC)
  target_link_libraries(${LIB_STATIC} ${COMMON_OBJLIB})
  foreach(P IN LISTS PREFIXES)
    target_link_libraries(${LIB_STATIC} "${P}_objlib")
  endforeach() 
  target_link_options(${LIB_STATIC} PRIVATE
    $<$<COMPILE_LANGUAGE:C>:${LDFLAGS}>)
  target_include_directories(${LIB_STATIC} INTERFACE
    $<INSTALL_INTERFACE:include>)
  install(TARGETS ${LIB_STATIC} DESTINATION lib)
endif()

if(JOMFFT_LIB_SHARED)
  set(LIB_SHARED "jomfft")
  add_library(${LIB_SHARED} SHARED)
  target_link_libraries(${LIB_SHARED} ${COMMON_OBJLIB})
  foreach(P IN LISTS PREFIXES)
    target_link_libraries(${LIB_SHARED} "${P}_objlib")
  endforeach() 
  target_link_options(${LIB_SHARED} PRIVATE
    $<$<COMPILE_LANGUAGE:C>:${LDFLAGS}>)
  target_include_directories(${LIB_SHARED} INTERFACE
    $<INSTALL_INTERFACE:include>)
  install(TARGETS ${LIB_SHARED} DESTINATION lib)
endif()

if(JOMFFT_BENCH_MKL)
  set(MKL_LINK static)
  if(JOMFFT_ENABLE_OPENMP)
    set(MKL_THREADING gnu_thread)
  else()
    set(MKL_THREADING sequential)
  endif()
  find_package(MKL CONFIG REQUIRED)
endif()

if(JOMFFT_TEST OR JOMFFT_BENCH OR JOMFFT_COMPARE)
  set(UTIL_CFLAGS "")
  if(JOMFFT_BENCH_FFTW OR JOMFFT_COMPARE)
    list(APPEND UTIL_CFLAGS "-DJOMFFT_USE_FFTW=1")
  endif()
  if(JOMFFT_BENCH_MKL)
    list(APPEND UTIL_CFLAGS "-DJOMFFT_USE_MKL=1")
  endif()
  foreach(P IN LISTS PREFIXES)
    set(UTIL_OBJLIB "${P}_util")
    if(${P} STREQUAL "jomfftf")
      set(CFLAGS ${CFLAGS_SINGLE})
    else()
      set(CFLAGS ${CFLAGS_DOUBLE})
    endif()
    add_library(${UTIL_OBJLIB} OBJECT "") 
    set_property(TARGET ${UTIL_OBJLIB} PROPERTY C_STANDARD 11)
    add_subdirectory(util "${CMAKE_CURRENT_BINARY_DIR}/${P}/util")
    target_include_directories(${UTIL_OBJLIB} PRIVATE include common dft)
    target_compile_options(${UTIL_OBJLIB} PRIVATE
      $<$<COMPILE_LANGUAGE:C>:${CFLAGS} ${UTIL_CFLAGS}>)
    if(JOMFFT_BENCH_MKL)
      target_include_directories(${UTIL_OBJLIB} BEFORE PRIVATE
        $<TARGET_PROPERTY:MKL::MKL,INTERFACE_INCLUDE_DIRECTORIES>)
    endif()
  endforeach()
endif()
    
set(LIBS "")
if(NOT (CMAKE_C_COMPILER_ID STREQUAL "MSVC"))
  list(APPEND LIBS "m")
endif()
if(JOMFFT_ENABLE_OPENMP)
  list(APPEND LIBS ${OpenMP_C_LIBRARIES})
endif()

if(JOMFFT_TEST)
  foreach(P IN LISTS PREFIXES)
    set(TEST "${P}_test")
    set(OBJLIB "${P}_objlib")
    set(UTIL_OBJLIB "${P}_util")
    if(${P} STREQUAL "jomfftf")
      set(CFLAGS ${CFLAGS_SINGLE})
    else()
      set(CFLAGS ${CFLAGS_DOUBLE})
    endif()
    add_executable(${TEST} $<TARGET_OBJECTS:${COMMON_OBJLIB}>
      $<TARGET_OBJECTS:${OBJLIB}> $<TARGET_OBJECTS:${UTIL_OBJLIB}>)
    set_property(TARGET ${TEST} PROPERTY C_STANDARD 11)
    add_subdirectory(test "${CMAKE_CURRENT_BINARY_DIR}/${P}/test")
    target_include_directories(${TEST} PRIVATE
      ${CMAKE_SOURCE_DIR} include common dft util)
    target_compile_options(${TEST} PRIVATE
      $<$<COMPILE_LANGUAGE:C>:${CFLAGS}>)
    target_link_options(${TEST} PRIVATE
      $<$<COMPILE_LANGUAGE:C>:${LDFLAGS}>)
    target_link_libraries(${TEST} PRIVATE ${LIBS})
  endforeach()
endif()

if(JOMFFT_LIB_TEST)
  foreach(P IN LISTS PREFIXES)
    set(LIBTEST "${P}_libtest")
    if(${P} STREQUAL "jomfftf")
      set(CFLAGS ${CFLAGS_SINGLE})
    else()
      set(CFLAGS ${CFLAGS_DOUBLE})
    endif()
    add_executable(${LIBTEST})
    set_property(TARGET ${LIBTEST} PROPERTY C_STANDARD 11)
    add_subdirectory(misc/libtest "${CMAKE_CURRENT_BINARY_DIR}/${P}/libtest")
    target_include_directories(${LIBTEST} PRIVATE include)
    target_compile_options(${LIBTEST} PRIVATE $<$<COMPILE_LANGUAGE:C>:${CFLAGS}>)
    target_link_libraries(${LIBTEST} PRIVATE jomfft_static ${LIBS})
  endforeach()
endif()

if(JOMFFT_COMPARE OR JOMFFT_BENCH_FFTW)
  list(APPEND LIBS fftw3 fftw3f)
  if(JOMFFT_ENABLE_OPENMP)
    list(APPEND LIBS fftw3_omp fftw3f_omp)
  endif()
endif()

if(JOMFFT_COMPARE)
  set(COMPARE_CFLAGS "-DJOMFFT_USE_FFTW=1")
  foreach(P IN LISTS PREFIXES)
    set(COMPARE "${P}_compare")
    set(OBJLIB "${P}_objlib")
    set(UTIL_OBJLIB "${P}_util")
    if(${P} STREQUAL "jomfftf")
      set(CFLAGS ${CFLAGS_SINGLE})
    else()
      set(CFLAGS ${CFLAGS_DOUBLE})
    endif()
    add_executable(${COMPARE} $<TARGET_OBJECTS:${COMMON_OBJLIB}>
      $<TARGET_OBJECTS:${OBJLIB}> $<TARGET_OBJECTS:${UTIL_OBJLIB}>)
    set_property(TARGET ${COMPARE} PROPERTY C_STANDARD 11)
    add_subdirectory(misc/compare "${CMAKE_CURRENT_BINARY_DIR}/${P}/compare")
    target_include_directories(${COMPARE} PRIVATE include common dft util)
    target_compile_options(${COMPARE} PRIVATE
      $<$<COMPILE_LANGUAGE:C>:${CFLAGS} ${COMPARE_CFLAGS}>)
    target_link_options(${COMPARE} PRIVATE
      $<$<COMPILE_LANGUAGE:C>:${LDFLAGS}>)
    target_link_libraries(${COMPARE} PRIVATE ${LIBS})
  endforeach()
endif()

set(CPATH "")
if(JOMFFT_BENCH_MKL)
  list(APPEND LIBS $<LINK_ONLY:MKL::MKL>)
  list(APPEND CPATH $<TARGET_PROPERTY:MKL::MKL,INTERFACE_INCLUDE_DIRECTORIES>)
endif()

if(JOMFFT_BENCH)
  if(JOMFFT_BENCH_FFTW)
    list(APPEND BENCH_CFLAGS "-DJOMFFT_USE_FFTW=1")
  endif()
  if(JOMFFT_BENCH_MKL)
    list(APPEND BENCH_CFLAGS "-DJOMFFT_USE_MKL=1")
  endif()
  foreach(P IN LISTS PREFIXES)
    set(BENCH "${P}_bench")
    set(OBJLIB "${P}_objlib")
    set(UTIL_OBJLIB "${P}_util")
    if(${P} STREQUAL "jomfftf")
      set(CFLAGS ${CFLAGS_SINGLE})
    else()
      set(CFLAGS ${CFLAGS_DOUBLE})
    endif()
    add_executable(${BENCH} $<TARGET_OBJECTS:${COMMON_OBJLIB}>
      $<TARGET_OBJECTS:${OBJLIB}> $<TARGET_OBJECTS:${UTIL_OBJLIB}>)
    set_property(TARGET ${BENCH} PROPERTY C_STANDARD 11)
    add_subdirectory(misc/bench "${CMAKE_CURRENT_BINARY_DIR}/${P}/bench")
    target_include_directories(${BENCH} PRIVATE include common dft util)
    target_compile_options(${BENCH} PRIVATE
      $<$<COMPILE_LANGUAGE:C>:${CFLAGS} ${BENCH_CFLAGS}>)
    target_link_options(${BENCH} PRIVATE
      $<$<COMPILE_LANGUAGE:C>:${LDFLAGS}>)
    target_link_libraries(${BENCH} PRIVATE ${LIBS})
    target_include_directories(${BENCH} BEFORE PRIVATE ${CPATH})
  endforeach()
endif()
