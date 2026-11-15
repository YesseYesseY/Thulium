#pragma once
#include "Actor.hpp"
#include "Pawn.hpp"
#include "PlayerState.hpp"

class AController : public AActor
{
    STATIC_CLASS(L"/Script/Engine.Controller");

    CLASS_PROP(APlayerState*, PlayerState);
    CLASS_PROP(APawn*, Pawn);

    FRotator GetControlRotation()
    {
        UFUNC("GetControlRotation");

        FRotator Ret;
        ProcessEvent(Func, &Ret);
        return Ret;
    }

public:
    template <typename T>
    T* GetPlayerState()
    {
        return (T*)PlayerState;
    }

    template <typename T>
    T* GetPawn()
    {
        return (T*)Pawn;
    }
};
