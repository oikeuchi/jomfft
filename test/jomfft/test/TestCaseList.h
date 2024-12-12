#pragma once

#include "Test_.h"

#include "TestCase.h"

#define LIST NS(TestCaseList)
#define LIST_(n) NS(TestCaseList_##n)
#define VALUE_TYPE struct NS(TestCase)

#include <jomfft/util/template/ListDef.h>

#include <jomfft/util/template/UndefListMacro.h>
