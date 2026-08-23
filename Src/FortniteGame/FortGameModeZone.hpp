#pragma once
#include "FortGameMode.hpp"
#include "ItemAndCount.hpp"

class AFortGameModeZone : public AFortGameMode
{
    STATIC_CLASS(L"/Script/FortniteGame.FortGameModeZone");

    CLASS_PROP(TArray<FItemAndCount>, StartingItems);
};
