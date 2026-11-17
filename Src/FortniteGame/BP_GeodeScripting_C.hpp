#pragma once
#include "../Engine/Actor.hpp"

class ABP_GeodeScripting_C : public AActor
{
    STATIC_CLASS(ABP_GeodeScripting_C);

    void TestLaunch(float Seconds)
    {
        UFUNC("TestLaunch");
        ProcessEvent(Func, &Seconds);
    }
};
