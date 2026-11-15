#include "FortPickupAthena.hpp"
#include "../Engine/World.hpp"

AFortPickup* AFortPickup::Spawn(FVector Pos)
{
    auto Ret = UWorld::GetWorld()->SpawnActor<AFortPickupAthena>(Pos);
    Ret->PickupLocationData.LootInitialPosition = Pos;
    Ret->OnRep_PickupLocationData();
    return Ret;
}
