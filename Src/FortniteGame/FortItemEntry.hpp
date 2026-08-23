#pragma once
#include "FortItemDefinition.hpp"

struct FFortItemEntry
{
    STATIC_STRUCT(FFortItemEntry, L"/Script/FortniteGame.FortItemEntry");

    STRUCT_PROP(UFortItemDefinition*, ItemDefinition);
    STRUCT_PROP(FGuid, ItemGuid);
};
