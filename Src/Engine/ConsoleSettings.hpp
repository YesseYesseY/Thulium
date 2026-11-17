#pragma once
#include "../CoreUObject.hpp"

// TODO Move to EngineSettings

struct FAutoCompleteCommand
{
    FString Command;
    FString Desc;
    FColor Color;
};

class UConsoleSettings : public UObject
{
    STATIC_CLASS(UConsoleSettings);

    CLASS_PROP(TArray<FAutoCompleteCommand>, ManualAutoCompleteList);
    CLASS_PROP(FColor, AutoCompleteCommandColor);
    CLASS_PROP(FColor, AutoCompleteCVarColor);
    CLASS_PROP(FColor, AutoCompleteFadedColor);
};
