#include "IntVector.h"

#include "IntUtil.h"

struct jomfftIntVector {
  Long size;
  Long capacity;
  int *data;
};

struct jomfftIntVector *jomfftIntVector_new() {
  struct jomfftIntVector *out = NEW(struct jomfftIntVector);
  *out = (struct jomfftIntVector){.size = 0, .capacity = 0, .data = NULL};
  return out;
}

void jomfftIntVector_delete(struct jomfftIntVector *p) {
  if (p) {
    jomfftIntVector_clear(p);
    DELETE(p);
  }
}

Long jomfftIntVector_size(struct jomfftIntVector *p) { return p->size; }

void jomfftIntVector_clear(struct jomfftIntVector *p) {
  assert(p);
  if (p->data) {
    ARRAY_DELETE(p->data);
    p->size = 0;
    p->capacity = 0;
    p->data = NULL;
  }
}

void jomfftIntVector_resize(struct jomfftIntVector *p, Long size) {
  assert(p);
  if (size <= p->capacity) {
    p->size = size;
  } else {
    Long old_capacity = p->capacity;
    Long capacity = MAX(old_capacity, 4);
    while (capacity < size) {
      capacity *= 2;
    }
    if (p->data) {
      int *new_data = ARRAY_NEW(int, capacity);
      ARRAY_COPY(int, p->size, p->data, new_data);
      ARRAY_DELETE(p->data);
      p->data = new_data;
    } else {
      p->data = ARRAY_NEW(int, capacity);
    }
    p->size = size;
    p->capacity = capacity;
  }
}

void jomfftIntVector_append(struct jomfftIntVector *p, int x) {
  jomfftIntVector_resize(p, p->size + 1);
  *(jomfftIntVector_end(p) - 1) = x;
}

const int *jomfftIntVector_cat(const struct jomfftIntVector *p, Long i) {
  assert(p);
  assert(0 <= i && i < p->size);
  return p->data + i;
}

int *jomfftIntVector_at(struct jomfftIntVector *p, Long i) {
  return (int *)(jomfftIntVector_cat(p, i));
}

const int *jomfftIntVector_cbegin(const struct jomfftIntVector *p) {
  assert(p);
  return p->data;
}

const int *jomfftIntVector_cend(const struct jomfftIntVector *p) {
  assert(p);
  return p->data + p->size;
}

int *jomfftIntVector_begin(struct jomfftIntVector *p) {
  return (int *)(jomfftIntVector_cbegin(p));
}

int *jomfftIntVector_end(struct jomfftIntVector *p) {
  return (int *)(jomfftIntVector_cend(p));
}

int *jomfftIntVector_data(struct jomfftIntVector *p) { return p->data; }

void jomfftIntVector_sort(struct jomfftIntVector *p) {
  assert(p);
  jomfftInt_Sort(p->size, p->data);
}

Long jomfftIntVector_binarySearch(const struct jomfftIntVector *p, int value) {
  assert(p);
  return jomfftInt_BinarySearch(p->size, p->data, value);
}
