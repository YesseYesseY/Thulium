#pragma once
#include "../CoreUObject.hpp"
#include "../Hook.hpp"
#include "NetDriver.hpp"
#include "LevelCollection.hpp"
#include "GameInstance.hpp"
#include "GameplayStatics.hpp"
#include "GameModeBase.hpp"

enum class ESpawnActorCollisionHandlingMethod : uint8
{
    Undefined = 0,
    AlwaysSpawn = 1,
    AdjustIfPossibleButAlwaysSpawn = 2,
    AdjustIfPossibleButDontSpawnIfColliding = 3,
    DontSpawnIfColliding = 4
};

class AActor;
class APawn;

struct FActorSpawnParameters
{
    FName Name;
    AActor* Template;
    AActor* Owner;
    APawn* Instigator;
    void* OverrideLevel; // ULevel
    ESpawnActorCollisionHandlingMethod SpawnCollisionHandlingOverride;
    uint16 bRemoteOwned : 1;
    uint16 bNoFail : 1;
    uint16 bDeferConstruction : 1;
    uint16 bAllowDuringConstructionScript : 1;
    EObjectFlags ObjectFlags;

    FActorSpawnParameters()
        :Name(FName()), Template(nullptr), Owner(nullptr), Instigator(nullptr), OverrideLevel(nullptr),
        SpawnCollisionHandlingOverride(ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn),
        bRemoteOwned(false), bNoFail(false), bDeferConstruction(false), bAllowDuringConstructionScript(false), ObjectFlags(EObjectFlags::Transactional)
    {
    }
};

class UWorld : public UObject
{
    STATIC_CLASS(UWorld);

    CLASS_PROP(UNetDriver*, NetDriver);
    CLASS_PROP(TArray<FLevelCollection>, LevelCollections);
    CLASS_PROP(UGameInstance*, OwningGameInstance);
    CLASS_PROP(AGameModeBase*, AuthorityGameMode);

    template <typename T = AGameModeBase>
    T* GetGameMode()
    {
        return (T*)AuthorityGameMode;
    }

private:
    static inline AActor* (*_SpawnActor)(UWorld*, UClass*, FTransform&, FActorSpawnParameters&) = nullptr;
    static inline bool (*_Exec)(UWorld*, int64, const wchar_t*, int64) = nullptr;

public:
    AActor* SpawnActor(UClass* ActorClass, FTransform translivesmatter)
    {
        FActorSpawnParameters Params = FActorSpawnParameters();
        return _SpawnActor(this, ActorClass, translivesmatter, Params);
    }

    template <typename T = AActor>
    T* SpawnActor(UClass* ActorClass, FVector Translation = {}, FRotator Rotation = {})
    {
        FTransform translivesmatter = FTransform(Translation, Rotation, { 1, 1, 1 });
        return (T*)SpawnActor(ActorClass, translivesmatter);
    }

    template <typename T>
    T* SpawnActor(FTransform translivesmatter)
    {
        return SpawnActor(T::StaticClass(), translivesmatter);
    }

    template <typename T>
    T* SpawnActor(FVector Translation = {}, FRotator Rotation = {})
    {
        return (T*)SpawnActor(T::StaticClass(), Translation, Rotation);
    }

public:
    static int64 GetNetModeHook(UWorld* This)
    {
        return 1;
    }

    static bool ExecHook(UWorld* This, int64 a2, const wchar_t* Cmd, int64 a4)
    {
        auto Msg = std::wstring(Cmd);

        if (Msg == L"givemecheats")
        {
            auto Controller = This->OwningGameInstance->LocalPlayers[0]->PlayerController;
            Controller->CheatManager = UGameplayStatics::SpawnObject<UCheatManager>(Controller);
            return true;
        }

        return _Exec(This, a2, Cmd, a4);
    }

public:
    static UWorld* GetWorld();

    static void Init()
    {
#if SERVER
        // GetNetMode
        {
            auto Addr = Memcury::Scanner(UObject::FindFunction(L"/Script/Engine.KismetSystemLibrary:IsDedicatedServer")->Func)
                .ScanForOpCode(0xE8, 2).RelativeOffset(1)
                .ScanForOpCode(0xE8, 1).RelativeOffset(1).Get();

            if (!Addr)
            {
                MsgBox("Failed to find UWorld::GetNetMode");
                return;
            }

            Hook::Function(Addr, GetNetModeHook);
        }

        // SpawnActor
        {
            uintptr_t Addr = 0;
            auto Scanner = Memcury::Scanner::FindStringRef(L"STAT_SpawnActorTime");
            if (Scanner.IsValid())
            {
                auto StrAddr = Scanner.Get();

                Scanner.ScanFor({ 0x4C, 0x8B, 0xDC }, false);

                auto NewAddr = Scanner.Get();
                if (StrAddr != NewAddr)
                    Addr = NewAddr;
            }

            if (!Addr)
            {
                MsgBox("Failed to find UWorld::SpawnActor");
                return;
            }

            _SpawnActor = decltype(_SpawnActor)(Addr);
        }
#else
        // UWorld::Exec
        {
            auto Addr = Memcury::Scanner::FindStringRef(L"TOGGLECVAR: cvar '%s' was not found")
                .ScanForOpCode(0xC3)
                .ScanForOpCode(0xE8)
                .RelativeOffset(1)
                .Get();

            if (Addr)
            {
                Hook::Function(Addr, ExecHook, &_Exec);
            }
            else
            {
                MsgBox("Failed to find UWorld::Exec, client commands will not work.");
            }
        }
#endif

    }

public:
    void Listen();

};
