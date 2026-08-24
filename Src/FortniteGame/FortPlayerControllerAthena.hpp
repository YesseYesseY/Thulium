#pragma once
#include "FortPlayerControllerPvP.hpp"
#include "FortGameModeAthena.hpp"
#include "FortKismetLibrary.hpp"
#include "BP_GeodeScripting_C.hpp"

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

    static void ServerCheatHook(AFortPlayerControllerAthena* This, const FString& FMsg)
    {
        auto Msg = FMsg.ToWString();

        if (Msg.starts_with(L"server "))
        {
            UKismetSystemLibrary::ExecuteConsoleCommand(Msg.substr(7).c_str());
        }
        else if (Msg == L"dumpobjects")
        {
            UObject::Objects->Dump();
            MsgBox("Dumped Objects");
        }
        else if (Msg == L"devloadout")
        {
            static std::vector<std::pair<UFortItemDefinition*, int32>> Items = {
                { UFortKismetLibrary::K2_GetResourceItemDefinition(EFortResourceType::Wood), 999 },
                { UFortKismetLibrary::K2_GetResourceItemDefinition(EFortResourceType::Stone), 999 },
                { UFortKismetLibrary::K2_GetResourceItemDefinition(EFortResourceType::Metal), 999 },

                { UObject::FindObject<UFortItemDefinition>(L"/Game/Athena/Items/Ammo/AthenaAmmoDataShells.AthenaAmmoDataShells"), 999 },
                { UObject::FindObject<UFortItemDefinition>(L"/Game/Athena/Items/Ammo/AthenaAmmoDataEnergyCell.AthenaAmmoDataEnergyCell"), 999 },
                { UObject::FindObject<UFortItemDefinition>(L"/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsMedium.AthenaAmmoDataBulletsMedium"), 999 },
                { UObject::FindObject<UFortItemDefinition>(L"/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsLight.AthenaAmmoDataBulletsLight"), 999 },
                { UObject::FindObject<UFortItemDefinition>(L"/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsHeavy.AthenaAmmoDataBulletsHeavy"), 999 },
                { UObject::FindObject<UFortItemDefinition>(L"/Game/Athena/Items/Ammo/AmmoDataRockets.AmmoDataRockets"), 999 },
                { UObject::FindObject<UFortItemDefinition>(L"/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_SR_Ore_T03.WID_Assault_AutoHigh_Athena_SR_Ore_T03"), 1 },
                { UObject::FindObject<UFortItemDefinition>(L"/Game/Athena/Items/Weapons/WID_Shotgun_SlugFire_Athena_SR.WID_Shotgun_SlugFire_Athena_SR"), 1 },
                { UObject::FindObject<UFortItemDefinition>(L"/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03"), 1 },
                { UObject::FindObject<UFortItemDefinition>(L"/Game/Athena/Items/Consumables/KnockGrenade/Athena_KnockGrenade.Athena_KnockGrenade"), 1 },
                { UObject::FindObject<UFortItemDefinition>(L"/Game/Athena/Items/Consumables/Shields/Athena_Shields.Athena_Shields"), 3 },
            };

            auto Inventory = This->WorldInventory;
            for (auto& thing : Items)
                Inventory->AddItem(thing.first, thing.second);
        }
        else if (Msg == L"startevent")
        {
            if (GameVersion != 4.5f)
                return;

            ABP_GeodeScripting_C::Get()->TestLaunch(60.0f);
        }
    }

public:
    static void Init()
    {
        auto Class = StaticClass();
        Class->HookVTable("ServerAcknowledgePossession", ServerAcknowledgePossessionHook);
        Class->HookVTable("ServerExecuteInventoryItem", ServerExecuteInventoryItemHook);
        Class->HookVTable("ServerLoadingScreenDropped", ServerLoadingScreenDroppedHook, &ServerLoadingScreenDroppedOriginal);
        Class->HookVTable("ServerCheat", ServerCheatHook);
    }
};
