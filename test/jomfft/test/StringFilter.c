#include "StringFilter.h"

#include "Test.h"

struct NS(StringFilter) *NS(StringFilter_new)(NS(StringFilter_fn) evaluate,
                                              jomfftDestruct_fn destruct,
                                              void *data) {
  struct NS(StringFilter) *out = NEW(struct NS(StringFilter));
  *out = (struct NS(StringFilter)){
      .evaluate = evaluate, .destruct = destruct, .data = data};
  return out;
}

void NS(StringFilter_delete)(struct NS(StringFilter) *p) {
  if (p) {
    if (p->destruct) {
      p->destruct(p->data);
      p->data = NULL;
    }
    DELETE(p);
  }
}

bool NS(StringFilter_evaluate)(struct NS(StringFilter) *p, const char *name) {
  assert(p);
  return p->evaluate(p, name);
}

static bool ExactMatchFilter_evaluate(struct NS(StringFilter) *p,
                                      const char *name) {
  assert(p);
  assert(p->data);
  const char *s = (const char *)p->data;
  return !strcmp(s, name);
}

struct NS(StringFilter) *NS(ExactMatchFilter_new)(const char *filter) {
  char *data = ARRAY_NEW(char, strlen(filter) + 1);
  strcpy(data, filter);
  return NS(StringFilter_new)(ExactMatchFilter_evaluate, jomfftDestructArray,
                              data);
}

static bool PartialMatchFilter_evaluate(struct NS(StringFilter) *p,
                                        const char *name) {
  assert(p);
  assert(p->data);
  const char *s = (const char *)p->data;
  return !strstr(s, name);
}

struct NS(StringFilter) *NS(PartialMatchFilter_new)(const char *filter) {
  char *data = ARRAY_NEW(char, strlen(filter) + 1);
  strcpy(data, filter);
  return NS(StringFilter_new)(PartialMatchFilter_evaluate, jomfftDestructArray,
                              data);
}

#if USE_REGEX

static bool RegexFilter_evaluate(struct NS(StringFilter) *p, const char *name) {
  assert(p);
  assert(p->data);
  regex_t *re = (regex_t *)p->data;
  return regexec(re, name, 0, NULL, 0) == 0;
}

static void RegexFilter_destruct(void *p) {
  if (p) {
    regex_t *re = (regex_t *)p;
    regfree(re);
    DELETE(re);
  }
}

struct NS(StringFilter) *NS(RegexFilter_new)(const char *filter) {
  regex_t *data = NEW(regex_t);
  if (regcomp(data, filter, 0)) {
    fprintf(stderr, "Invalid regex '%s'.\n", filter);
    DELETE(data);
    return NULL;
  }
  return NS(StringFilter_new)(RegexFilter_evaluate, RegexFilter_destruct, data);
}

#endif
