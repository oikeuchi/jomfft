#include <jomfft/Version.h>

#include "CommonTest.h"

void NS(VersionTest)() {
  ASSERT_INT_EQ(jomfftGetVersionMajor(), JOMFFT_VERSION_MAJOR);
  ASSERT_INT_EQ(jomfftGetVersionMinor(), JOMFFT_VERSION_MINOR);
}

void NS(MaxThreadsTest)() {
#if USE_OPENMP
  ASSERT_INT_EQ(jomfftGetMaxThreads(), omp_get_max_threads());
  jomfftSetMaxThreads(1);
  ASSERT_INT_EQ(jomfftGetMaxThreads(), 1);
  jomfftSetMaxThreads(0);
  ASSERT_INT_EQ(jomfftGetMaxThreads(), omp_get_max_threads());
#else
  ASSERT_INT_EQ(jomfftGetMaxThreads(), 1);
#endif
}
