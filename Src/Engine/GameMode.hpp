#pragma once
#include "GameModeBase.hpp"

class AGameMode : public AGameModeBase
{
    STATIC_CLASS(AGameMode);

    CLASS_PROP(int32, NumPlayers);
};
