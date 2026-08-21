#include <format>
#include <Windows.h>
#include "Hook.hpp"
#include "UnrealContainers.hpp"

#include "Engine/Engine.hpp"
#include "Engine/GameplayStatics.hpp"
#include "Engine/KismetSystemLibrary.hpp"

#include "FortniteGame/FortGameModeAthena.hpp"
#include "FortniteGame/FortPlayerState.hpp"

#include "OnlineSubsystemUtils/OnlineBeaconHost.hpp"

void Init()
{
    MH_Initialize();

    // FMemory::Realloc
    {
        auto Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 48 8B F1 41 8B D8 48 8B 0D").Get();

        if (!Addr)
        {
            MsgBox("Failed to find FMemory::Realloc");
            return;
        }

        FMemory::Init(Addr);
    }

    // RequestExit
    {
        auto Addr = Memcury::Scanner::FindPattern("40 53 48 83 EC 30 80 3D ? ? ? ? ? 0F B6 D9").Get();

        if (!Addr)
        {
            MsgBox("Failed to find FPlatformMisc::RequestExit");
        }
        else
        {
            Hook::Function(Addr, Hook::ReturnHook);
        }
    }

    UObject::Init();
    UStruct::Init();
    UClass::Init();
    UProperty::Init();
    UBoolProperty::Init();
    UFunction::Init();

#if SERVER
    UEngine::Init();
    UWorld::Init();
    UNetDriver::Init();
    UReplicationDriver::Init();

    AFortGameModeAthena::Init();
    AFortPlayerState::Init();

    AOnlineBeacon::Init();
    AOnlineBeaconHost::Init();

    // GIsClient/GIsServer
    {
        // Oddities i've found with GIsClient+Server
        //  - Some builds use 0xC6 mov, some use 0x88
        //  - 4.5-CL-4159770: random 0x88 mov between Client and Server
        auto Scanner = Memcury::Scanner::FindStringRef(L"STAT_PlatformInit");

        std::vector<uint8> OpCodes = { 0xC6, 0x88 };
        auto RelOff = 2;
        auto AbsOff = 1;
        uint8 SecondByte = 0;
        uint8 ThirdByte = 0;

        Scanner.ScanForEitherOpCode(OpCodes);
        auto GIsClientMov = Scanner.Get();
        SecondByte = *(uint8*)(int64(GIsClientMov) + 1);
        ThirdByte = *(uint8*)(int64(GIsClientMov) + 2);
        if (SecondByte == 0x88)
        {
            RelOff = 3;
            AbsOff = 0;
        }

        *Memcury::PE::Address(GIsClientMov).RelativeOffset(RelOff).AbsoluteOffset(AbsOff).GetAs<bool*>() = false;

        Scanner.ScanForEitherOpCode(OpCodes);

        if (*(uint8*)(Scanner.Get() + 2) != ThirdByte)
            Scanner.ScanForEitherOpCode(OpCodes);

        auto GIsServerMov = Scanner.Get();
        *Memcury::PE::Address(GIsServerMov).RelativeOffset(RelOff).AbsoluteOffset(AbsOff).GetAs<bool*>() = true;
    }

    UWorld::GetWorld()->OwningGameInstance->LocalPlayers.Remove(0);

    // KickPlayer
    {
        auto Addr = Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 49 8B F0 48 8B DA 48 85 D2").Get();

        if (!Addr)
        {
            MsgBox("Failed to find AGameSession::KickPlayer");
            return;
        }

        Hook::Function(Addr, Hook::ReturnFalse);
    }

    // 3.5 grass crash
    {
        auto Scanner = Memcury::Scanner::FindStringRef(L"STAT_GrassUpdate");
        if (Scanner.IsValid())
            Hook::Function(Scanner.ScanFor({ 0x4C, 0x8B, 0xDC }, false).Get(), Hook::ReturnHook);
    }

    // STAT_PoiVolume_CheckPawnOverlap crash
    {
        auto Scanner = Memcury::Scanner::FindStringRef(L"STAT_PoiVolume_CheckPawnOverlap");
        if (Scanner.IsValid())
            Hook::Function(Scanner.ScanForOpCode(0xE8).RelativeOffset(1).Get(), Hook::ReturnHook);
    }
#endif
}

DWORD MainThread(void*)
{
    Init();

    // MsgBox("{:X}", UObject::FindFunction(L"/Script/Engine.GameModeBase:SpawnDefaultPawnFor")->GetVTableIndex());

#if CLIENT
    auto Viewport = UEngine::GetEngine()->GameViewport;
    Viewport->ViewportConsole = UGameplayStatics::SpawnObject<UConsole>(Viewport);
#endif

    // UObject::Objects->Dump();
    // MsgBox("Dumped Objects");

    // MsgBox("{}", UKismetSystemLibrary::GetEngineVersion());

#if CLIENT
#define MapString L"open 127.0.0.1"
#else
#define MapString L"open Athena_Terrain"
#endif
    UKismetSystemLibrary::ExecuteConsoleCommand(MapString);

    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
        CreateThread(0, 0, MainThread, 0, 0, 0);

    return true;
}
