#include "BluesteinDftDataList.h"

#define LIST NS(BluesteinDftDataList)
#define LIST_(n) NS(BluesteinDftDataList_##n)
#define KEY_TYPE struct NS(BluesteinDftDataKey)
#define KEY_TYPE_(n) NS(BluesteinDftDataKey_##n)
#define VALUE_TYPE struct NS(BluesteinDftData)
#define LIST_VALUE_DELETE NS(BluesteinDftData_delete)

#include "../util/template/KeyedListImpl.h"
