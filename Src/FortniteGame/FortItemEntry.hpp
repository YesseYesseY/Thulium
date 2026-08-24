#pragma once
#include "../Engine/FastArraySerializerItem.hpp"
#include "FortItemDefinition.hpp"

struct FFortItemEntry : public FFastArraySerializerItem
{
    STATIC_STRUCT(FFortItemEntry, L"/Script/FortniteGame.FortItemEntry");

    STRUCT_PROP(UFortItemDefinition*, ItemDefinition);
    STRUCT_PROP(FGuid, ItemGuid);
    STRUCT_PROP(int32, Count);
};
