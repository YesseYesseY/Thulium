#pragma once
#include "Controller.hpp"
#include "CheatManager.hpp"

class APawn;

class APlayerController : public AController
{
    STATIC_CLASS(L"/Script/Engine.PlayerController");

    CLASS_PROP(APawn*, AcknowledgedPawn);
    CLASS_PROP(UObject*, CheatManager);
};
