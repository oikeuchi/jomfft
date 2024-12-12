#include "Test.h"

#include <jomfft/util/random/Random.h>

#include "Capture.h"
#include "CaptureList.h"
#include "StringFilter.h"
#include "TestCase.h"
#include "TestCaseList.h"

static struct NS(TestCaseList) *g_tests = NULL;

static struct NS(TestCase) *g_current_test = NULL;

static struct NS(CaptureList) *g_captures = NULL;

static int g_test_count = 0;
static int g_success_count = 0;
static int g_failure_count = 0;

static struct NS(CaptureList) *GetCaptureList() {
  if (!g_captures) {
    g_captures = NS(CaptureList_new)();
  }
  return g_captures;
}

static void ClearCaptureList() {
  if (g_captures) {
    NS(CaptureList_clear)(g_captures);
  }
}

void NS(Test_Begin)(unsigned int seed) {
  NS(Random_Init)(seed);
  NS(Test_Clear)();
  g_test_count = 0;
  g_success_count = 0;
  g_failure_count = 0;
}

int NS(Test_End)() {
  if (g_tests) {
    NS(TestCaseList_delete)(g_tests);
    g_tests = NULL;
  }
  if (g_captures) {
    NS(CaptureList_delete)(g_captures);
    g_captures = NULL;
  }
  if (g_failure_count > 0) {
    printf("Test failed!!!\n");
  } else {
    printf("Test succeeded.\n");
  }
  printf("Total %d Tests with %d Assertions (Success=%d, Failure=%d) \n.",
         g_test_count, g_success_count + g_failure_count, g_success_count,
         g_failure_count);
  fflush(stdout);
  return g_failure_count > 0 ? 1 : 0;
}

void NS(Test_Clear)() {
  if (g_tests) {
    NS(TestCaseList_clear)(g_tests);
  }
  ClearCaptureList();
}

void NS(Test_Register)(const char *name, NS(Test_fn) func) {
  if (!g_tests) {
    g_tests = NS(TestCaseList_new)();
  }
  struct NS(TestCase) *o = NEW(struct NS(TestCase));
  *o = (struct NS(TestCase)){.name = name, .func = func};
  NS(TestCaseList_append)(g_tests, o);
}

void NS(Test_Run)(const char *filter, bool regex) {
  struct NS(StringFilter) *filter_ = NULL;
  if (filter) {
    if (regex) {
#if USE_REGEX
      filter_ = NS(RegexFilter_new)(filter);
#else
      filter_ = NS(PartialMatchFilter_new)(filter);
#endif
    } else {
      filter_ = NS(ExactMatchFilter_new)(filter);
    }
  }
  struct NS(TestCaseList_Node) *it = NS(TestCaseList_head)(g_tests);
  for (; it; it = NS(TestCaseList_next)(it)) {
    struct NS(TestCase) *o = NS(TestCaseList_data)(it);
    if (filter_ && !NS(StringFilter_evaluate)(filter_, o->name)) {
      continue;
    }
    ClearCaptureList();
    g_current_test = o;
    o->func();
    g_current_test = NULL;
    ++g_test_count;
  }
  if (filter_) {
    NS(StringFilter_delete)(filter_);
  }
}

void NS(Test_ReportSuccess)() { ++g_success_count; }

void NS(Test_ReportFailure)(bool print_error) {
  ++g_failure_count;
  if (g_current_test && print_error) {
    printf("Test '%s' failed.\n", g_current_test->name);
  }
}

void NS(Test_Capture)(const char *name, const char *file, int lineno,
                      struct NS(Variadic) *value) {
  struct NS(CaptureList) *list = GetCaptureList();
  struct NS(CaptureKey) key =
      (struct NS(CaptureKey)){.name = name, .file = file, .lineno = lineno};
  NS(CaptureList_set)(list, key, value);
}

void NS(Test_ClearCapture)() { ClearCaptureList(); }

void NS(Test_PrintCapture)() {
  if (!g_captures) {
    return;
  }
  printf("Capture:\n");
  struct NS(CaptureList_Node) *it = NS(CaptureList_head)(g_captures);
  for (; it; it = NS(CaptureList_next)(it)) {
    NS(Capture_print)(NS(CaptureList_key)(it), NS(CaptureList_data(it)));
  }
}
