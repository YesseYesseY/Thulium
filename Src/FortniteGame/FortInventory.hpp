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
    void Update(FFortItemEntry* ItemEntry = nullptr)
    {
        HandleInventoryLocalUpdate();
        if (ItemEntry)
            Inventory.MarkItemDirty(ItemEntry);
        else
            Inventory.MarkArrayDirty();
    }

    FFortItemEntry* FindItemEntry(const FGuid& ItemGuid, int32* Idx = nullptr)
    {
        auto& Items = Inventory.ReplicatedEntries;
        for (int i = 0; i < Items.Num(); i++)
        {
            auto& Entry = Items.Get(i, FFortItemEntry::Size());
            if (Entry.ItemGuid == ItemGuid)
            {
                if (Idx)
                    *Idx = i;
                return &Entry;
            }
        }

        return nullptr;
    }

    inline int32 Num() const
    {
        return Inventory.ReplicatedEntries.Num();
    }

    void AddItem(UFortItemDefinition* ItemDef, int32 Count, bool ShouldUpdate = true);
    bool RemoveItem(const FGuid& ItemGuid, int32 Count);
};
