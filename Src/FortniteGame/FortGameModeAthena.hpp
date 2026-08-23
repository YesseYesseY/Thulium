#pragma once
#include "FortGamePvPBase.hpp"
#include "FortGameStateAthena.hpp"
#include "FortPlayerController.hpp"
#include "FortPlayerState.hpp"
#include "FortAbilitySet.hpp"

class AFortGameModeAthena : public AFortGamePvPBase
{
    STATIC_CLASS(L"/Script/FortniteGame.FortGameModeAthena");

    CLASS_PROP(int32, WarmupRequiredPlayerCount);

    static APawn* SpawnDefaultPawnFor(AGameModeBase* This, AFortPlayerController* NewPlayer, AActor* StartSpot)
    {
        NewPlayer->WorldInventory->Update();
        return This->SpawnDefaultPawnAtTransform(NewPlayer, StartSpot->GetTransform());
    }

    static bool ReadyToStartMatch(AFortGameModeAthena* This)
    {
        static bool Inited = false;
        if (!Inited)
        {
            Inited = true;

            auto Playlist = UObject::FindObject<UFortPlaylistAthena>(L"/Game/Athena/Playlists/Playlist_DefaultSolo.Playlist_DefaultSolo");
            auto GameState = This->GetGameState<AFortGameStateAthena>();

            GameState->CurrentPlaylistData = Playlist;
            GameState->OnRep_CurrentPlaylistData();

            // This for some reason stops the server from freezing after calling OnRep_CurrentPlaylistData on older builds
            Sleep(2000);

            UWorld::GetWorld()->Listen();

            This->WarmupRequiredPlayerCount = 1;
        }

        if (This->NumPlayers > 0)
        {
            This->bWorldIsReady = true;
            return true;
        }

        return false;
    }

    static inline void (*HandleStartingNewPlayerOriginal)(AFortGameModeAthena* This, APlayerController* PlayerController);
    static void HandleStartingNewPlayerHook(AFortGameModeAthena* This, AFortPlayerController* Controller)
    {
        HandleStartingNewPlayerOriginal(This, Controller);

        auto PlayerState = Controller->GetPlayerState<AFortPlayerState>();
        auto Pawn = Controller->GetPawn<AFortPawn>();
        PlayerState->ApplyCustomizationToCharacter(Pawn);

        static auto AS = UObject::FindObject<UFortAbilitySet>(L"/Game/Abilities/Player/Generic/Traits/DefaultPlayer/GAS_DefaultPlayer.GAS_DefaultPlayer");
        AS->Give(PlayerState->AbilitySystemComponent);
    }

    static void Init()
    {
        auto Class = StaticClass();
        Class->HookVTable("SpawnDefaultPawnFor", SpawnDefaultPawnFor);
        Class->HookVTable("ReadyToStartMatch", ReadyToStartMatch);
        Class->HookVTable("HandleStartingNewPlayer", HandleStartingNewPlayerHook, &HandleStartingNewPlayerOriginal);
    }
};
