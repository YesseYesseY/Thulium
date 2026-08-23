#pragma once
#include "FortPlayerControllerPvP.hpp"
#include "FortGameModeAthena.hpp"

class AFortPlayerControllerAthena : public AFortPlayerControllerPvP
{
    STATIC_CLASS(L"/Script/FortniteGame.FortPlayerControllerAthena");

public:
    static void ServerAcknowledgePossessionHook(AFortPlayerControllerAthena* This, APawn* Pawn)
    {
        This->AcknowledgedPawn = Pawn;
    }

    static void ServerExecuteInventoryItemHook(AFortPlayerControllerAthena* This, const FGuid& ItemGuid)
    {
        if (auto ItemEntry = This->WorldInventory->FindItemEntry(ItemGuid))
            This->GetPawn<AFortPawn>()->EquipItemEntry(ItemEntry);
    }

    static inline void (*ServerLoadingScreenDroppedOriginal)(AFortPlayerControllerAthena*) = nullptr;
    static void ServerLoadingScreenDroppedHook(AFortPlayerControllerAthena* This)
    {
        ServerLoadingScreenDroppedOriginal(This);

        // I want this in HandleStartingNewPlayer but it gets buggy on builds below 4.4
        static auto StartingItems = UWorld::GetWorld()->GetGameMode<AFortGameModeAthena>()->StartingItems;
        static auto Pickaxe = UObject::FindObject<UFortItemDefinition>(L"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Athena_C_T01.WID_Harvest_Pickaxe_Athena_C_T01");
        auto Inventory = This->WorldInventory;
        if (Inventory->Num() <= 0)
        {
            Inventory->AddItem(Pickaxe, 1);
            for (auto& thing : StartingItems)
                Inventory->AddItem(thing.Item, thing.Count);
        }
    }

public:
    static void Init()
    {
        auto Class = StaticClass();
        Class->HookVTable("ServerAcknowledgePossession", ServerAcknowledgePossessionHook);
        Class->HookVTable("ServerExecuteInventoryItem", ServerExecuteInventoryItemHook);
        Class->HookVTable("ServerLoadingScreenDropped", ServerLoadingScreenDroppedHook, &ServerLoadingScreenDroppedOriginal);
    }
};
