#pragma once

#include "Data.h"

#include "BluesteinDftData.h"

#define LIST NS(BluesteinDftDataList)
#define LIST_(n) NS(BluesteinDftDataList_##n)
#define KEY_TYPE struct NS(BluesteinDftDataKey)
#define VALUE_TYPE struct NS(BluesteinDftData)

#include "../util/template/KeyedListDef.h"

#include "../util/template/UndefKeyedListMacro.h"
