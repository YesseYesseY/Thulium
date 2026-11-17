#pragma once
#include "FortGameMode.hpp"
#include "ItemAndCount.hpp"

class AFortGameModeZone : public AFortGameMode
{
    STATIC_CLASS(AFortGameModeZone);

    CLASS_PROP(TArray<FItemAndCount>, StartingItems);
};
