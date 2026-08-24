#pragma once
#include "BuildingActor.hpp"

class ABuildingSMActor : public ABuildingActor
{
    STATIC_CLASS(L"/Script/FortniteGame.BuildingSMActor");

    void SetMirrored(bool bIsMirrored)
    {
        UFUNC("SetMirrored");
        ProcessEvent(Func, &bIsMirrored);
    }
};
