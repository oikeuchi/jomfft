#include "Common.h"

#include "Version.h"

JOMFFT_API int jomfftGetVersionMajor() { return JOMFFT_VERSION_MAJOR; }

JOMFFT_API int jomfftGetVersionMinor() { return JOMFFT_VERSION_MINOR; }

#if USE_OPENMP

static int g_max_thread = 0;

JOMFFT_API int jomfftGetMaxThreads() {
  if (g_max_thread <= 0) {
    return omp_get_max_threads();
  } else {
    return g_max_thread;
  }
}

JOMFFT_API jomfftStatus jomfftSetMaxThreads(int num_threads) {
  g_max_thread = MIN(num_threads, omp_get_max_threads());
  return jomfftSuccess;
}

#else

JOMFFT_API int jomfftGetMaxThreads() { return 1; }

JOMFFT_API jomfftStatus jomfftSetMaxThreads(int num_threads) {
  if (num_threads > 1) {
    fprintf(stderr, "Warning: '%s' is called but Open MP is not supported.\n",
            __func__);
    return jomfftInvalidArgument;
  }
  return jomfftSuccess;
}

#endif
