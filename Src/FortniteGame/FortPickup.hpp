#pragma once
#include "../Engine/Actor.hpp"
#include "../Engine/ProjectileMovementComponent.hpp"
#include "../Engine/World.hpp"
#include "../Engine/KismetMathLibrary.hpp"
#include "FortItemEntry.hpp"
#include "FortPickupLocationData.hpp"
#include "BuildingContainer.hpp"

class AFortPickup : public AActor
{
    STATIC_CLASS(AFortPickup);

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

    void TossFromContainer(ABuildingContainer* Container)
    {
        auto World = UWorld::GetWorld();

        auto LootTossConeHalfAngle = Container->LootTossConeHalfAngle_Athena;
        auto LootTossDirection = Container->LootTossDirection_Athena;
        auto LootTossSpeed = Container->LootTossSpeed_Athena;

        float v12 = -0.5f;
        int v13 = -((int)(float)(-0.5f - (float)((float)(LootTossConeHalfAngle * (1.0f/18.0f)) + (float)(LootTossConeHalfAngle * (1.0f/18.0f)))) >> 1);
        int v14 = -((int)(float)(-0.5f - (float)((float)((float)v13 * 0.5f) + (float)((float)v13 * 0.5f))) >> 1);
        if (v14 <= 1)
            v14 = 1;

        int Step = UKismetMathLibrary::RandomInteger(v13);

        auto ContainerRotation = Container->K2_GetActorRotation();
        auto xmmRot = *(__m128*)&ContainerRotation;
        float v21 = _mm_shuffle_ps(xmmRot, xmmRot, 85).m128_f32[0] + LootTossDirection.Yaw;

        FRotator v31 = { 0.0f, 0.0f, 0.0f };
        v31.Pitch = ContainerRotation.Pitch + LootTossDirection.Pitch;
        v31.Roll = ContainerRotation.Roll + LootTossDirection.Roll;

        float v22 = 1.0f / (float)v14;
        float v23 = (float)((float)(Step / 2 % v14 + 1) * LootTossConeHalfAngle) * v22;
        if (Step % 2 == 1)
            v23 *= -1.0f;
        v31.Yaw = v21 + v23;

        float v24 = (float)(LootTossConeHalfAngle * 0.004363323f) * v22;
        float v25 = (float)((float)-(((Step - 1) / (2 * v14) + 1)
                         / ((int)(float)(v12
                                       - (float)((float)((float)v13 / (float)(2 * v14))
                                               + (float)((float)v13 / (float)(2 * v14)))) >> 1))
                * (float)(LootTossSpeed - (float)(LootTossSpeed * 0.7f)))
        + (float)(LootTossSpeed * 0.7f);

        FVector v26 = v31.Vector();
        auto v27 = UKismetMathLibrary::RandomUnitVectorInEllipticalConeInRadians(v26, v24, 1.0f);
        FVector Vel = FVector(v25 * v27.X, v25 * v27.Y, v25 * v27.Z);

        TossVelocity(Vel);
        TossedFromContainer();
    }

public:
    static AFortPickup* Spawn(FVector Pos);
};
