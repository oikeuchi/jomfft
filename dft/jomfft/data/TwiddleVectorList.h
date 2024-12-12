#pragma once

#include "Data.h"
#include "TwiddleVector.h"

#define LIST NS(TwiddleVectorList)
#define LIST_(n) NS(TwiddleVectorList_##n)
#define KEY_TYPE struct NS(TwiddleVectorKey)
#define VALUE_TYPE Complex

#include "../util/template/KeyedListDef.h"

#include "../util/template/UndefKeyedListMacro.h"
