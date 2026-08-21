#include "Engine.hpp"
#include "../FortniteGame/FortOnlineBeaconHost.hpp"

UNetDriver* UEngine::CreateNetDriver(UWorld* World, FName Def)
{
    auto Beacon = UWorld::GetWorld()->SpawnActor<AFortOnlineBeaconHost>();
    Beacon->ListenPort = 7776;
    if (!Beacon->InitHost())
    {
        MsgBox("Failed AOnlineBeaconHost::InitHost");
        return nullptr;
    }

    Beacon->PauseRequests(false);

    auto NetDriver = Beacon->NetDriver;
    NetDriver->NetDriverName = L"GameNetDriver";
    World->NetDriver = NetDriver;

    return NetDriver;
}
