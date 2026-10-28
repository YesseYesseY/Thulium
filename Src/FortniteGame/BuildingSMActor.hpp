#pragma once
#include "BuildingActor.hpp"
#include "FortPlayerStateZone.hpp"

class ABuildingSMActor : public ABuildingActor
{
    STATIC_CLASS(L"/Script/FortniteGame.BuildingSMActor");

    CLASS_PROP(AFortPlayerStateZone*, EditingPlayer);

private:
    static inline ABuildingSMActor* (*RBA)(ABuildingSMActor*, uint8, UClass*, int, int, bool, AFortPlayerController*) = nullptr;

public:
    ABuildingSMActor* ReplaceBuildingActor(UClass* NewClass, int32 RotationIterations, bool bMirrored, AFortPlayerController* PlayerController)
    {
        return RBA(this, 1, NewClass, 0, RotationIterations, bMirrored, PlayerController);
    }

public:
    void SetMirrored(bool bIsMirrored)
    {
        UFUNC("SetMirrored");
        ProcessEvent(Func, &bIsMirrored);
    }

    void OnRep_EditingPlayer()
    {
        UFUNC("OnRep_EditingPlayer");
        ProcessEvent(Func);
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

    static void Init()
    {
        // ABuildingSMActor::ReplaceBuildingActor TODO Find this better
        {
            auto Addr = Memcury::Scanner::FindStringRef(L"STAT_Fort_BuildingSMActorReplaceBuildingActor")
                .ScanForEither({{ 0x4C, 0x8B, 0xDC }, { 0x48, 0x8B, 0xC4 }}, false).Get();

            RBA = decltype(RBA)(Addr);
        }
    }
};
