#pragma once
#include "../Engine/Actor.hpp"
#include "../Engine/ProjectileMovementComponent.hpp"
#include "FortItemEntry.hpp"
#include "FortPickupLocationData.hpp"

class AFortPickup : public AActor
{
    STATIC_CLASS(L"/Script/FortniteGame.FortPickup");

    CLASS_PROP(FFortItemEntry, PrimaryPickupItemEntry);
    CLASS_PROP(UProjectileMovementComponent*, MovementComponent);
    CLASS_PROP(bool, bTossedFromContainer);
    CLASS_PROP(FFortPickupLocationData, PickupLocationData);
    CLASS_PROP_SAFE(bool, bTossed);
    CLASS_PROP_SAFE(uint32, PickupSourceTypeFlags);

    void OnRep_PrimaryPickupItemEntry()
    {
        UFUNC("OnRep_PrimaryPickupItemEntry");
        ProcessEvent(Func);
    }

    void OnRep_PickupLocationData()
    {
        UFUNC("OnRep_PickupLocationData");
        ProcessEvent(Func);
    }

public:
    void SetItemDef(UFortItemDefinition* ItemDef, int32 Count = -1)
    {
        PrimaryPickupItemEntry.ItemDefinition = ItemDef;
        if (Count != -1)
            PrimaryPickupItemEntry.Count = Count;
        OnRep_PrimaryPickupItemEntry();
    }

    void Tossed()
    {
        if (bTossed_Exists())
            bTossed = true;
        if (PickupSourceTypeFlags_Exists())
            PickupSourceTypeFlags |= 16; // Tossed
    }

    // TODO
    void TossedFromContainer()
    {
        bTossedFromContainer = true;
        if (PickupSourceTypeFlags_Exists())
            PickupSourceTypeFlags |= 4; // Container
    }

    void TossVelocity(FVector Velocity)
    {
        // Reversed from 0x1199230 on 7.30-CL-4834550
        if (IsActorTickEnabled())
            SetNetDormancy(1);
        else
            FlushNetDormancy();
        bReplicateMovement = true;
        NetPriority = 3.0f;

        if (MovementComponent)
        {
            MovementComponent->Velocity = Velocity;
            MovementComponent->MaxSpeed = std::sqrt((Velocity.X * Velocity.X) + (Velocity.Y * Velocity.Y) + (Velocity.Z * Velocity.Z));
        }

        Tossed();
        // TODO Func 0x1192370 on 7.30-CL-4834550
        ForceNetUpdate();
    }

public:
    static AFortPickup* Spawn(FVector Pos);
};
