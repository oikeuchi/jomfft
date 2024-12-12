#pragma once

#include "Util_.h"

static int optind = 1;
static int opterr = 1;
static int optopt = '?';
static char *optarg;

static int getopt(int argc, char **argv, const char *optstring) {
  int pos = optind;
  while (pos < argc && argv[pos][0] != '-') {
    ++pos;
  }
  if (pos == argc) {
    return -1;
  }
  char *arg = argv[pos];
  if (arg[1] == '-') {
    return -1;
  }
  const char *opt = strchr(optstring, arg[1]);
  if (!opt) {
    optopt = arg[1];
    if (opterr && optstring[0] != ':') {
      fprintf(stderr, "%s: invalid option -- '%c'\n", argv[0], optopt);
    }
    return '?';
  }
  ++pos;
  int has_optarg = (int)(opt[1] == ':');
  if (has_optarg) {
    if (pos == argc) {
      optopt = arg[1];
      if (opterr && optstring[0] != ':') {
        fprintf(stderr, "%s: option requires an argument -- '%c'\n", argv[0],
                optopt);
      }
      return '?';
    }
    optarg = argv[pos];
    ++pos;
  }
  for (int i = pos - 1; i > optind + has_optarg; --i) {
    argv[i] = argv[i - has_optarg - 1];
  }
  argv[optind] = arg;
  ++optind;
  if (has_optarg) {
    argv[optind] = optarg;
    ++optind;
  }
  return *opt;
}
