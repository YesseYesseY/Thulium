#pragma once
#include "LocalPlayer.hpp"

class UGameInstance : public UObject
{
    STATIC_CLASS(L"/Script/Engine.GameInstance");

    CLASS_PROP(TArray<ULocalPlayer*>, LocalPlayers);
};
