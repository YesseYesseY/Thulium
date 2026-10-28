#pragma once
#include "FortWeap_BuildingToolBase.hpp"
#include "BuildingSMActor.hpp"

class AFortWeap_EditingTool : public AFortWeap_BuildingToolBase
{
    STATIC_CLASS(L"/Script/FortniteGame.FortWeap_EditingTool");

    CLASS_PROP(ABuildingSMActor*, EditActor);

    void OnRep_EditActor()
    {
        UFUNC("OnRep_EditActor");
        ProcessEvent(Func);
    }
};
