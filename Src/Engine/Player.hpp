#pragma once
#include "../CoreUObject.hpp"
#include "PlayerController.hpp"

class UPlayer : public UObject
{
    STATIC_CLASS(UPlayer);

    CLASS_PROP(APlayerController*, PlayerController);
};
