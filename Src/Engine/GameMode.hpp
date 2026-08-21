#pragma once
#include "GameModeBase.hpp"

class AGameMode : public AGameModeBase
{
    STATIC_CLASS(L"/Script/Engine.GameMode");

    CLASS_PROP(int32, NumPlayers);
};
