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
