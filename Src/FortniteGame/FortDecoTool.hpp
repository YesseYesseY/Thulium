#pragma once
#include "FortWeapon.hpp"

class AFortDecoTool : public AFortWeapon
{
    STATIC_CLASS(L"/Script/FortniteGame.FortDecoTool");

    CLASS_PROP(UFortItemDefinition*, ItemDefinition);

    void OnRep_ItemDefinition()
    {
        UFUNC("OnRep_ItemDefinition");
        ProcessEvent(Func);
    }
};
