#include "TestCaseList.h"

#define LIST NS(TestCaseList)
#define LIST_(n) NS(TestCaseList_##n)
#define VALUE_TYPE struct NS(TestCase)
#define LIST_VALUE_DELETE NS(TestCase_delete)

#include <jomfft/util/template/ListImpl.h>
