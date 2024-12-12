#pragma once

#include "Test_.h"
#include <jomfft/Destruct.h>

struct NS(StringFilter);

typedef bool (*NS(StringFilter_fn))(struct NS(StringFilter) *p,
                                    const char *name);

struct NS(StringFilter) {
  NS(StringFilter_fn) evaluate;
  jomfftDestruct_fn destruct;
  void *data;
};

struct NS(StringFilter) *NS(StringFilter_new)(NS(StringFilter_fn) evaluate,
                                              jomfftDestruct_fn destruct,
                                              void *data);

void NS(StringFilter_delete)(struct NS(StringFilter) *p);

bool NS(StringFilter_evaluate)(struct NS(StringFilter) *p, const char *name);

struct NS(StringFilter) *NS(ExactMatchFilter_new)(const char *filter);

struct NS(StringFilter) *NS(PartialMatchFilter_new)(const char *filter);

struct NS(StringFilter) *NS(RegexFilter_new)(const char *filter);
