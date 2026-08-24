#pragma once
#include "../Engine/Actor.hpp"

class ABP_GeodeScripting_C : public AActor
{
    STATIC_CLASS(L"/Game/Athena/Maps/Test/Events/BP_GeodeScripting.BP_GeodeScripting_C");
    CLASS_GET(ABP_GeodeScripting_C);

    void TestLaunch(float Seconds)
    {
        UFUNC("TestLaunch");
        ProcessEvent(Func, &Seconds);
    }
};
