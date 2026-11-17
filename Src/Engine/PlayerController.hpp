#pragma once
#include "Controller.hpp"
#include "CheatManager.hpp"

class APawn;

class APlayerController : public AController
{
    STATIC_CLASS(APlayerController);

    CLASS_PROP(APawn*, AcknowledgedPawn);
    CLASS_PROP(UObject*, CheatManager);
    CLASS_PROP(FVector, LastSpectatorSyncLocation);
    CLASS_PROP(FRotator, LastSpectatorSyncRotation);

public:
    static void GetPlayerViewPointHook(APlayerController* This, FVector* Location, FRotator* Rotation)
    {
        static auto NAME_Spectating = FName(L"Spectating");
        if (This->StateName == NAME_Spectating)
        {
            *Location = This->LastSpectatorSyncLocation;
            *Rotation = This->LastSpectatorSyncRotation;
        }
        else if (auto Pawn = This->Pawn)
        {
            *Location = Pawn->K2_GetActorLocation();
            *Rotation = This->GetControlRotation();
        }
        else
        {
            *Location = This->K2_GetActorLocation();
            *Rotation = This->K2_GetActorRotation();
        }
    }

public:
    static void Init()
    {
        // GetPlayerViewPoint
        {
            // TODO Use UFunc when it becomes available
            auto Idx = *Memcury::Scanner::FindStringRef(L"BUGSCREENSHOTWITHHUDINFO %s")
                .ScanForOpCode(0xFF, 1)
                .AbsoluteOffset(2)
                .GetAs<int32*>() / 8;

            StaticClass()->HookAllVTables(Idx, GetPlayerViewPointHook);
        }
    }
};
