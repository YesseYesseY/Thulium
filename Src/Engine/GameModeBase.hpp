#pragma once
#include "Controller.hpp"
#include "Pawn.hpp"
#include "GameStateBase.hpp"

class AGameModeBase : public AActor
{
    STATIC_CLASS(L"/Script/Engine.GameModeBase");

    CLASS_PROP(AGameStateBase*, GameState);
    CLASS_PROP(UClass*, DefaultPawnClass);

    template <typename T>
    T* GetGameState()
    {
        return (T*)GameState;
    }

    APawn* SpawnDefaultPawnAtTransform(AController* NewPlayer, const FTransform& SpawnTransform)
    {
        UFUNC("SpawnDefaultPawnAtTransform");

        struct {
            AController* NP;
            FTransform ST;
            APawn* Ret;
        } Args { NewPlayer, SpawnTransform };

        ProcessEvent(Func, &Args);

        return Args.Ret;
    }
};
