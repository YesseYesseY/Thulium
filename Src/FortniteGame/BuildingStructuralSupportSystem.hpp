#pragma once
#include "../CoreUObject.hpp"
#include "FortStructuralGridQueryResults.hpp"

class UBuildingStructuralSupportSystem : public UObject
{
    STATIC_CLASS(L"/Script/FortniteGame.BuildingStructuralSupportSystem");
    CLASS_GET(UBuildingStructuralSupportSystem);

private:
    EFortStructuralGridQueryResults CanAddBuildingActorClassToGrid(UClass* BuildingSMActorClassToCheck, const FVector& Location, const FRotator& Rotation, bool bMirrored, TArray<ABuildingActor*>* OutExistingBuildings, bool bAllowStaticOverlap)
    {
        UFUNC("CanAddBuildingActorClassToGrid");

        struct {
            UObject* WorldContextObject;
            UClass* BuildingSMActorClassToCheck;
            FVector Location;
            FRotator Rotation;
            bool bMirrored;
            TArray<ABuildingActor*> OutExistingBuildings;
            bool bAllowStaticOverlap;
            EFortStructuralGridQueryResults Ret;
        } Args { UWorld::GetWorld(), BuildingSMActorClassToCheck, Location, Rotation, bMirrored };
        ProcessEvent(Func, &Args);

        if (OutExistingBuildings)
            *OutExistingBuildings = Args.OutExistingBuildings;
        else
            Args.OutExistingBuildings.Free();

        return Args.Ret;
    }

public:
    bool CanPlaceBuildingClass(UClass* BuildingClass, const FVector& Location, const FRotator& Rotation, bool bMirrored, TArray<ABuildingActor*>* ExistingBuildings)
    {
        UFUNC("CanAddBuildingActorClassToGrid");
        if (Func)
            return CanAddBuildingActorClassToGrid(BuildingClass, Location, Rotation, bMirrored, ExistingBuildings, false) == EFortStructuralGridQueryResults::CanAdd;

        return true; // TODO A couple 'sudo pacman -Syu' later and suddenly all builds below 4.5 stop working! :D
    }

public:
    static void Init()
    {
    }
};
