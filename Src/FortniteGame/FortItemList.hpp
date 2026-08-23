#pragma once
#include "../Engine/FastArraySerializer.hpp"
#include "FortItemEntry.hpp"
#include "FortWorldItem.hpp"

struct FFortItemList : public FFastArraySerializer
{
    STATIC_STRUCT(FFortItemList, L"/Script/FortniteGame.FortItemList");

    STRUCT_PROP(TArray<FFortItemEntry>, ReplicatedEntries);
    STRUCT_PROP(TArray<UFortWorldItem*>, ItemInstances);
};
