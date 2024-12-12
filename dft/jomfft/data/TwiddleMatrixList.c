#include "TwiddleMatrixList.h"

#define LIST NS(TwiddleMatrixList)
#define LIST_(n) NS(TwiddleMatrixList_##n)
#define KEY_TYPE struct NS(TwiddleMatrixKey)
#define KEY_TYPE_(n) NS(TwiddleMatrixKey_##n)
#define VALUE_TYPE Complex
#define LIST_VALUE_DELETE NS(TwiddleMatrix_delete)

#include "../util/template/KeyedListImpl.h"
