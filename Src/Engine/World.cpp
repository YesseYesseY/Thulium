#include "World.hpp"
#include "Engine.hpp"

UWorld* UWorld::GetWorld()
{
    return UEngine::GetEngine()->GameViewport->World;
}

void UWorld::Listen()
{
    auto Engine = UEngine::GetEngine();

    Engine->CreateNetDriver(this, L"GameNetDriver");

    auto URL = FURL::New();
    URL->Port = 7777;

    FString Error;
    if (!NetDriver->InitListen(this, URL, false, Error))
    {
        MsgBox("Failed NetDriver->InitListen");
        return;
    }

    NetDriver->SetWorld(this);
    LevelCollections.Get(0, FLevelCollection::Size()).NetDriver = NetDriver;
    LevelCollections.Get(1, FLevelCollection::Size()).NetDriver = NetDriver;
}
