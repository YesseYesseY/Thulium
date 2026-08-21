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
};
