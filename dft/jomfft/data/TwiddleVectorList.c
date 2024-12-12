#include "TwiddleVectorList.h"

#define LIST NS(TwiddleVectorList)
#define LIST_(n) NS(TwiddleVectorList_##n)
#define KEY_TYPE struct NS(TwiddleVectorKey)
#define KEY_TYPE_(n) NS(TwiddleVectorKey_##n)
#define VALUE_TYPE Complex
#define LIST_VALUE_DELETE NS(TwiddleVector_delete)

#include "../util/template/KeyedListImpl.h"
