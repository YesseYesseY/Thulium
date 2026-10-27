#pragma once
#include "BuildingActor.hpp"

class ABuildingSMActor : public ABuildingActor
{
    STATIC_CLASS(L"/Script/FortniteGame.BuildingSMActor");

    void SetMirrored(bool bIsMirrored)
    {
        UFUNC("SetMirrored");
        ProcessEvent(Func, &bIsMirrored);
    }

    void RepairBuilding(AFortPlayerController* RepairingController, int32 ResourcesSpent)
    {
        UFUNC("RepairBuilding");
        struct
        {
            AFortPlayerController* RepairingController;
            int32 ResourcesSpent;
        } Args { RepairingController, ResourcesSpent };
        ProcessEvent(Func, &Args);
    }
};
