#include "BuildingContainer.hpp"
#include "FortPickup.hpp"
#include "FortKismetLibrary.hpp"
#include "../Engine/KismetMathLibrary.hpp"

void ABuildingContainer::SpawnLootHook(ABuildingContainer* This)
{
    static std::vector<std::pair<UFortItemDefinition*, int32>> things = {
        { UFortKismetLibrary::K2_GetResourceItemDefinition(EFortResourceType::Wood), 30 },
        { UObject::FindObject<UFortItemDefinition>(L"/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsMedium.AthenaAmmoDataBulletsMedium"), 14 },
        { UObject::FindObject<UFortItemDefinition>(L"/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_SR_Ore_T03.WID_Assault_AutoHigh_Athena_SR_Ore_T03"), 1 },
        { UObject::FindObject<UFortItemDefinition>(L"/Game/Athena/Items/Consumables/Shields/Athena_Shields.Athena_Shields"), 1 },
    };

    auto Pos = UKismetMathLibrary::TransformLocation(This->GetTransform(), This->LootSpawnLocation_Athena);

    for (auto& thing : things)
    {
        auto Pickup = AFortPickup::Spawn(Pos);
        Pickup->SetItemDef(thing.first, thing.second);
        Pickup->TossFromContainer(This);
    }
}
