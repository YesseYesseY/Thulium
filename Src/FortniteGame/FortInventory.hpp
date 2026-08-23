#pragma once
#include "../Engine/Actor.hpp"
#include "FortItemList.hpp"
#include "FortItemDefinition.hpp"

class AFortInventory : public AActor
{
    STATIC_CLASS(L"/Script/FortniteGame.FortInventory");

    CLASS_PROP(FFortItemList, Inventory);

    void HandleInventoryLocalUpdate()
    {
        UFUNC("HandleInventoryLocalUpdate");
        ProcessEvent(Func);
    }

public:
    void Update()
    {
        HandleInventoryLocalUpdate();
        Inventory.MarkArrayDirty();
    }

    FFortItemEntry* FindItemEntry(const FGuid& ItemGuid)
    {
        auto& Items = Inventory.ReplicatedEntries;
        for (int i = 0; i < Items.Num(); i++)
        {
            auto& Entry = Items.Get(i, FFortItemEntry::Size());
            if (Entry.ItemGuid == ItemGuid)
                return &Entry;
        }

        return nullptr;
    }

    inline int32 Num() const
    {
        return Inventory.ReplicatedEntries.Num();
    }

    void AddItem(UFortItemDefinition* ItemDef, int32 Count, bool ShouldUpdate = true);
};
