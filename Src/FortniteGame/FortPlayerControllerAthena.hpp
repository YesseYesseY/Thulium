#pragma once
#include "FortPlayerControllerPvP.hpp"

class AFortPlayerControllerAthena : public AFortPlayerControllerPvP
{
    STATIC_CLASS(L"/Script/FortniteGame.FortPlayerControllerAthena");

public:
    static void ServerAcknowledgePossession(AFortPlayerControllerAthena* This, APawn* Pawn)
    {
        This->AcknowledgedPawn = Pawn;
    }

public:
    static void Init()
    {
        auto Class = StaticClass();
        Class->HookVTable("ServerAcknowledgePossession", ServerAcknowledgePossession);
    }
};
