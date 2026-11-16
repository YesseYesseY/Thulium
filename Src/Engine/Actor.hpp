#pragma once
#include "../CoreUObject.hpp"
#include "SceneComponent.hpp"

class AActor : public UObject
{
    STATIC_CLASS(L"/Script/Engine.Actor");

    CLASS_BIT(bReplicateMovement);
    CLASS_PROP(float, NetPriority);
    CLASS_PROP(USceneComponent*, RootComponent);

    FTransform GetTransform()
    {
        UFUNC("GetTransform");

        FTransform Ret;
        ProcessEvent(Func, &Ret);
        return Ret;
    }

    void ForceNetUpdate()
    {
        UFUNC("ForceNetUpdate");
        ProcessEvent(Func);
    }

    void K2_DestroyActor()
    {
        UFUNC("K2_DestroyActor");
        ProcessEvent(Func);
    }

    FVector K2_GetActorLocation()
    {
        UFUNC("K2_GetActorLocation");

        FVector Ret;
        ProcessEvent(Func, &Ret);
        return Ret;
    }

    FRotator K2_GetActorRotation()
    {
        UFUNC("K2_GetActorRotation");

        FRotator Ret;
        ProcessEvent(Func, &Ret);
        return Ret;
    }

    AActor* GetOwner()
    {
        UFUNC("GetOwner");

        AActor* Ret;
        ProcessEvent(Func, &Ret);
        return Ret;
    }

    bool IsActorTickEnabled()
    {
        UFUNC("IsActorTickEnabled");

        bool Ret;
        ProcessEvent(Func, &Ret);
        return Ret;
    }

    void SetNetDormancy(uint8 NewDormancy)
    {
        UFUNC("SetNetDormancy");
        ProcessEvent(Func, &NewDormancy);
    }

    void FlushNetDormancy()
    {
        UFUNC("FlushNetDormancy");
        ProcessEvent(Func);
    }

    bool K2_TeleportTo(FVector DestLocation, FRotator DestRotation = { 0.0f, 0.0f, 0.0f })
    {
        UFUNC("K2_TeleportTo");

        struct {
            FVector DestLocation;
            FRotator DestRotation;
            bool Ret;
        } Args { DestLocation, DestRotation };
        ProcessEvent(Func, &Args);
        return Args.Ret;
    }

    template <typename T>
    T* GetOwner()
    {
        return (T*)GetOwner();
    }
};
