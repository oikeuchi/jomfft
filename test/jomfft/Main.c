#include <jomfft/util/GetOpt.h>

#include "RegisterTest.h"
#include "Test.h"

int main(int argc, char **argv) {
  const char *filter = NULL;
  unsigned int seed = 123456U;
  bool use_regex = false;
  {
    int opt = 0;
    while ((opt = getopt(argc, argv, "s:eh")) != -1) {
      switch (opt) {
      case 's':
        seed = (unsigned int)atoi(optarg);
        break;
      case 'e':
        use_regex = true;
        break;
      case 'h':
        printf("Usage: jomfft_test (<filter>)\n"
               "  <filter>: test case name to execute (run all if not "
               "specified).\n"
               "  -s <seed>: seed for random number generation.\n"
               "  -e: use regex for filtering test cases.\n"
               "  -h: this message.\n");
        return 0;
      default:
        return -1;
      }
    }
  }
  if (optind < argc) {
    filter = argv[optind];
  }
  NS(Test_Begin(seed));
  NS(RegisterTest)();
  NS(Test_Run(filter, use_regex));
  return NS(Test_End)();
}
