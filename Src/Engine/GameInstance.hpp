#pragma once
#include "LocalPlayer.hpp"

class UGameInstance : public UObject
{
    STATIC_CLASS(UGameInstance);

    CLASS_PROP(TArray<ULocalPlayer*>, LocalPlayers);
};
