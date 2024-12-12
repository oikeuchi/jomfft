#pragma once

#include "Test_.h"

struct NS(TestCase) {
  const char *name;
  NS(Test_fn) func;
};

struct NS(TestCase) *NS(TestCase_new)(const char *name, NS(Test_fn) func);

void NS(TestCase_delete)(struct NS(TestCase) *p);
