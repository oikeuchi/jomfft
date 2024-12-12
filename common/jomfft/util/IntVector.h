#pragma once

#include "../Common.h"

struct jomfftIntVector;

struct jomfftIntVector *jomfftIntVector_new();

void jomfftIntVector_delete(struct jomfftIntVector *p);

Long jomfftIntVector_size(struct jomfftIntVector *p);

void jomfftIntVector_clear(struct jomfftIntVector *p);

void jomfftIntVector_resize(struct jomfftIntVector *p, Long size);

void jomfftIntVector_append(struct jomfftIntVector *p, int x);

int *jomfftIntVector_append_(struct jomfftIntVector *p);

const int *jomfftIntVector_cat(const struct jomfftIntVector *p, Long i);

int *jomfftIntVector_at(struct jomfftIntVector *p, Long i);

const int *jomfftIntVector_cbegin(const struct jomfftIntVector *p);

const int *jomfftIntVector_cend(const struct jomfftIntVector *p);

int *jomfftIntVector_begin(struct jomfftIntVector *p);

int *jomfftIntVector_end(struct jomfftIntVector *p);

int *jomfftIntVector_data(struct jomfftIntVector *p);
void jomfftIntVector_sort(struct jomfftIntVector *p);

Long jomfftIntVector_binarySearch(const struct jomfftIntVector *p, int value);
