#pragma once

#include "Data.h"
#include "LargeTwiddleMatrix.h"

#define LIST NS(LargeTwiddleMatrixList)
#define LIST_(n) NS(LargeTwiddleMatrixList_##n)
#define KEY_TYPE struct NS(LargeTwiddleMatrixKey)
#define VALUE_TYPE Complex

#include "../util/template/KeyedListDef.h"

#include "../util/template/UndefKeyedListMacro.h"
