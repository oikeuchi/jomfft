#include "CaptureList.h"

#define LIST NS(CaptureList)
#define LIST_(n) NS(CaptureList_##n)
#define KEY_TYPE struct NS(CaptureKey)
#define KEY_TYPE_(n) NS(CaptureKey_##n)
#define VALUE_TYPE struct NS(Variadic)
#define LIST_VALUE_DELETE NS(Variadic_delete)

#include <jomfft/util/template/KeyedListImpl.h>
