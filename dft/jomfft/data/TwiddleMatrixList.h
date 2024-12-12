#pragma once

#include "Data.h"
#include "TwiddleMatrix.h"

#define LIST NS(TwiddleMatrixList)
#define LIST_(n) NS(TwiddleMatrixList_##n)
#define KEY_TYPE struct NS(TwiddleMatrixKey)
#define VALUE_TYPE Complex

#include "../util/template/KeyedListDef.h"

#include "../util/template/UndefKeyedListMacro.h"
