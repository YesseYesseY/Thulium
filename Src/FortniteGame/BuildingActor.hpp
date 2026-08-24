#pragma once
#include "../Engine/Actor.hpp"
#include "FortPlayerController.hpp"

class ABuildingActor : public AActor
{
    STATIC_CLASS(L"/Script/FortniteGame.BuildingActor");

    void InitializeKismetSpawnedBuildingActor(ABuildingActor* BuildingOwner, AFortPlayerController* SpawningController)
    {
        UFUNC("InitializeKismetSpawnedBuildingActor");

        struct {
            ABuildingActor* BuildingOwner;
            AFortPlayerController* SpawningController;
        } Args { BuildingOwner, SpawningController };
        ProcessEvent(Func, &Args);
    }

    void SilentDie()
    {
        UFUNC("SilentDie");
        ProcessEvent(Func);
    }
};
