#pragma once
#include "../CoreUObject.hpp"
#include "FortResourceType.hpp"
#include "FortResourceItemDefinition.hpp"

class UFortKismetLibrary : public UObject
{
    STATIC_CLASS(L"/Script/FortniteGame.FortKismetLibrary");
    DEFAULT_OBJ(UFortKismetLibrary);

    static UFortResourceItemDefinition* K2_GetResourceItemDefinition(EFortResourceType ResourceType)
    {
        UFUNC_STATIC("K2_GetResourceItemDefinition");
        struct {
            EFortResourceType ResourceType;
            UFortResourceItemDefinition* Ret;
        } Args { ResourceType };
        DefaultObj()->ProcessEvent(Func, &Args);
        return Args.Ret;
    }
};
