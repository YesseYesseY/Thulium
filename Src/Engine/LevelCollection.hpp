#pragma once
#include "NetDriver.hpp"

struct FLevelCollection
{
    STATIC_STRUCT(FLevelCollection, L"/Script/Engine.LevelCollection");

    STRUCT_PROP(UNetDriver*, NetDriver);
};
