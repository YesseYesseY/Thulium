#pragma once
#include "../CoreUObject.hpp"

class AActor : public UObject
{
    STATIC_CLASS(L"/Script/Engine.Actor");

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

    AActor* GetOwner()
    {
        UFUNC("GetOwner");

        AActor* Ret;
        ProcessEvent(Func, &Ret);
        return Ret;
    }

    template <typename T>
    T* GetOwner()
    {
        return (T*)GetOwner();
    }
};
