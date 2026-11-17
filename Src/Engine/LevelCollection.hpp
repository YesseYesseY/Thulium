#pragma once
#include "NetDriver.hpp"

struct FLevelCollection
{
    STATIC_STRUCT(FLevelCollection);

    STRUCT_PROP(UNetDriver*, NetDriver);
};
