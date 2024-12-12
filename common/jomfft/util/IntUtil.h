#pragma once

#include "../Common.h"

void jomfftInt_Sort(Long size, int *data);

Long jomfftInt_BinarySearch(Long size, const int *data, int key);

int jomfftInt_Product(int size, const int *data);

Long jomfftInt_ProductLong(Long size, const int *data);
