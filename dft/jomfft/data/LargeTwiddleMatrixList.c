#include "LargeTwiddleMatrixList.h"

#define LIST NS(LargeTwiddleMatrixList)
#define LIST_(n) NS(LargeTwiddleMatrixList_##n)
#define KEY_TYPE struct NS(LargeTwiddleMatrixKey)
#define KEY_TYPE_(n) NS(LargeTwiddleMatrixKey_##n)
#define VALUE_TYPE Complex
#define LIST_VALUE_DELETE NS(LargeTwiddleMatrix_delete)

#include "../util/template/KeyedListImpl.h"
