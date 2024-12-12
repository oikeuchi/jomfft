#pragma once

#include "Test_.h"

#include "Capture.h"

#define LIST NS(CaptureList)
#define LIST_(n) NS(CaptureList_##n)
#define KEY_TYPE struct NS(CaptureKey)
#define VALUE_TYPE struct NS(Variadic)

#include <jomfft/util/template/KeyedListDef.h>

#include <jomfft/util/template/UndefKeyedListMacro.h>
