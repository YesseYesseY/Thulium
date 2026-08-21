#pragma once
#include "FortGamePvPBase.hpp"
#include "FortGameStateAthena.hpp"
#include "FortPlayerController.hpp"
#include "FortPlayerState.hpp"

class AFortGameModeAthena : public AFortGamePvPBase
{
    STATIC_CLASS(L"/Script/FortniteGame.FortGameModeAthena");

    static APawn* SpawnDefaultPawnFor(AGameModeBase* This, AController* NewPlayer, AActor* StartSpot)
    {
        return This->SpawnDefaultPawnAtTransform(NewPlayer, StartSpot->GetTransform());
    }

    static bool ReadyToStartMatch(AFortGameModeAthena* This)
    {
        static auto StartClass = UObject::FindClass(L"/Script/FortniteGame.FortPlayerStartWarmup");
        if (UGameplayStatics::GetNumActorsOfClass(StartClass) <= 0)
            return false;

        static bool Inited = false;
        if (!Inited)
        {
            Inited = true;

            This->DefaultPawnClass = UObject::FindClass(L"/Game/Athena/PlayerPawn_Athena.PlayerPawn_Athena_C");

            auto Playlist = UObject::FindObject<UFortPlaylistAthena>(L"/Game/Athena/Playlists/Playlist_DefaultSolo.Playlist_DefaultSolo");
            auto GameState = This->GetGameState<AFortGameStateAthena>();
            // OnRep_CurrentPlaylistData freezes the server before 4.1
            // GameState->CurrentPlaylistId = Playlist->PlaylistId;
            // GameState->OnRep_CurrentPlaylistId();
            // GameState->CurrentPlaylistData = Playlist;
            // GameState->OnRep_CurrentPlaylistData();

            UWorld::GetWorld()->Listen();

            This->bWorldIsReady = true;
        }

        if (This->NumPlayers > 0)
            return true;

        return false;
    }

    static inline void (*HandleStartingNewPlayerOriginal)(AFortGameModeAthena* This, APlayerController* PlayerController);
    static void HandleStartingNewPlayerHook(AFortGameModeAthena* This, APlayerController* Controller)
    {
        HandleStartingNewPlayerOriginal(This, Controller);

        auto PlayerState = Controller->GetPlayerState<AFortPlayerState>();
        auto Pawn = Controller->GetPawn<AFortPawn>();
        PlayerState->ApplyCustomizationToCharacter(Pawn);
    }

    static void Init()
    {
        auto Class = StaticClass();
        Class->HookVTable("SpawnDefaultPawnFor", SpawnDefaultPawnFor);
        Class->HookVTable("ReadyToStartMatch", ReadyToStartMatch);
        Class->HookVTable("HandleStartingNewPlayer", HandleStartingNewPlayerHook, &HandleStartingNewPlayerOriginal);
    }
};
