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

    EFortStructuralGridQueryResults K2_CanAddBuildingActorToGrid(UObject* ActorToCheck, const FVector& Location, const FRotator& Rotation, bool bMirrored, TArray<ABuildingActor*>* OutExistingBuildings, bool bAllowStaticOverlap)
    {
        UFUNC("K2_CanAddBuildingActorToGrid");

        struct {
            UObject* WorldContextObject;
            UObject* ActorToCheck;
            FVector Location;
            FRotator Rotation;
            bool bMirrored;
            TArray<ABuildingActor*> OutExistingBuildings;
            bool bAllowStaticOverlap;
            EFortStructuralGridQueryResults Ret;
        } Args { UWorld::GetWorld(), ActorToCheck, Location, Rotation, bMirrored };
        ProcessEvent(Func, &Args);

        if (OutExistingBuildings)
            *OutExistingBuildings = Args.OutExistingBuildings;
        else
            Args.OutExistingBuildings.Free();

        return Args.Ret;
    }

    // 3.5 to 4.2?
    EFortStructuralGridQueryResults CanAddBuildingActorClassToGridOld(UClass* BuildingSMActorClassToCheck, const FVector& Location, const FRotator& Rotation, bool bMirrored, TArray<ABuildingActor*>* OutExistingBuildings, bool bAllowStaticOverlap)
    {
        static int64 (*thingy)(void*, void*, const FVector&, const FRotator&) = nullptr;
        if (thingy == nullptr)
        {
            auto Addr = Memcury::Scanner::FindPattern("48 89 74 24 ? 48 89 7C 24 ? 55 48 8B EC 48 81 EC 80 00 00 00 48 8B FA 48 8B F1").Get();

            if (!Addr)
            {
                MsgBox("Failed to find thingy in CanAddBuildingActorClassToGridOld");
                return EFortStructuralGridQueryResults::InvalidActor;
            }

            thingy = decltype(thingy)(Addr);
        }

        if (!BuildingSMActorClassToCheck)
            return EFortStructuralGridQueryResults::InvalidActor;

        auto Default = BuildingSMActorClassToCheck->DefaultObject;
        if (!Default)
            return EFortStructuralGridQueryResults::InvalidActor;

        // IsA check here

        FVector loc;
        thingy(Default, &loc, Location, Rotation);
        return K2_CanAddBuildingActorToGrid(Default, loc, Rotation, bMirrored, OutExistingBuildings, bAllowStaticOverlap);
    }

public:
    bool CanPlaceBuildingClass(UClass* BuildingClass, const FVector& Location, const FRotator& Rotation, bool bMirrored, TArray<ABuildingActor*>* ExistingBuildings)
    {
        UFUNC("CanAddBuildingActorClassToGrid");
        if (Func)
            return CanAddBuildingActorClassToGrid(BuildingClass, Location, Rotation, bMirrored, ExistingBuildings, false) == EFortStructuralGridQueryResults::CanAdd;

        return CanAddBuildingActorClassToGridOld(BuildingClass, Location, Rotation, bMirrored, ExistingBuildings, false) == EFortStructuralGridQueryResults::CanAdd;
    }

public:
    static void Init()
    {
    }
};
