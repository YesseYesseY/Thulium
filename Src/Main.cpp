#include <format>
#include <Windows.h>
#include "Hook.hpp"
#include "UnrealContainers.hpp"

#include "Engine/Engine.hpp"
#include "Engine/GameplayStatics.hpp"
#include "Engine/KismetSystemLibrary.hpp"

#include "FortniteGame/FortGameModeAthena.hpp"
#include "FortniteGame/FortPlayerState.hpp"
#include "FortniteGame/FortPlayerControllerAthena.hpp"

#include "OnlineSubsystemUtils/OnlineBeaconHost.hpp"

#include "GameplayAbilities/AbilitySystemComponent.hpp"

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
    UEnum::Init();

    // GameVersion/EngineVersion
    {
        auto VerStr = UKismetSystemLibrary::GetEngineVersion();
        EngineVersion = std::stof(VerStr);
        GameVersion = std::stof(VerStr.substr(VerStr.find_last_of('-') + 1));

        if (VerStr.starts_with("4.26.1"))
            EngineVersion = 4.261f;
    }

#if SERVER
    UEngine::Init();
    UWorld::Init();
    UNetDriver::Init();
    UReplicationDriver::Init();

    AFortGameModeAthena::Init();
    AFortPlayerControllerAthena::Init();
    AFortPlayerState::Init();

    AOnlineBeacon::Init();
    AOnlineBeaconHost::Init();

    UAbilitySystemComponent::Init();

    // GIsClient/GIsServer
    {
        // Oddities i've found with GIsClient+Server
        //  - Some builds use 0xC6 mov, some use 0x88
        //  - 4.5-CL-4159770: random 0x88 mov between Client and Server
        auto Scanner = Memcury::Scanner::FindStringRef(L"STAT_PlatformInit");

        std::vector<uint8> OpCodes = { 0xC6, 0x88 };
        uintptr_t MovClient = 0;
        uintptr_t MovServer = 0;
        hde64s Hde64Client;
        hde64s Hde64Server;

        Scanner.ScanForEitherOpCode(OpCodes);
        hde64_disasm(Scanner.GetAs<void*>(), &Hde64Client);
        MovClient = Scanner.Get();

        do
        {
            Scanner.ScanForEitherOpCode(OpCodes);
            hde64_disasm(Scanner.GetAs<void*>(), &Hde64Server);
        }
        while (Hde64Server.modrm != Hde64Client.modrm);
        MovServer = Scanner.Get();

        *(bool*)(MovClient + Hde64Client.len + *(int32*)&Hde64Client.disp.disp32) = false;
        *(bool*)(MovServer + Hde64Server.len + *(int32*)&Hde64Server.disp.disp32) = true;
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

    // MsgBox("{}", UObject::FindEnum(L"/Script/FortniteGame.EFortItemType")->GetValue("EventPurchaseTracker"));

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
    UKismetSystemLibrary::ExecuteConsoleCommand(L"log LogFort VeryVerbose");

    UKismetSystemLibrary::ExecuteConsoleCommand(MapString);

    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
        CreateThread(0, 0, MainThread, 0, 0, 0);

    return true;
}
