#pragma once
#include "../Engine/GameMode.hpp"

class AFortGameMode : public AGameMode
{
    STATIC_CLASS(L"/Script/FortniteGame.FortGameMode");

    CLASS_BIT(bWorldIsReady);
};
