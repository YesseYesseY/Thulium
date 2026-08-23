#pragma once
#include "../CoreUObject.hpp"
#include "PlayerController.hpp"

class UPlayer : public UObject
{
    STATIC_CLASS(L"/Script/Engine.Player");

    CLASS_PROP(APlayerController*, PlayerController);
};
