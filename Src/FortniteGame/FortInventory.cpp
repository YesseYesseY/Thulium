#include "FortInventory.hpp"
#include "FortPlayerController.hpp"

void AFortInventory::AddItem(UFortItemDefinition* ItemDef, int32 Count, bool ShouldUpdate)
{
    if (!ItemDef || !Count)
        return;

    auto Item = (UFortWorldItem*)ItemDef->CreateTemporaryItemInstanceBP(Count);
    if (!Item)
        return;

    Inventory.ItemInstances.Add(Item);
    Inventory.ReplicatedEntries.Add<FFortItemEntry>(Item->ItemEntry);

    if (ShouldUpdate)
        Update();
}

bool AFortInventory::RemoveItem(const FGuid& ItemGuid, int32 Count)
{
    int32 Idx = -1;
    auto ItemEntry = FindItemEntry(ItemGuid, &Idx);
    if (!ItemEntry || Idx == -1)
        return false;

    if (Count >= ItemEntry->Count)
    {
        Inventory.ItemInstances.Remove(Idx);
        Inventory.ReplicatedEntries.Remove<FFortItemEntry>(Idx);
        Update();
    }
    else
    {
        ItemEntry->Count -= Count;
        Update(ItemEntry);
    }

    return true;
}
