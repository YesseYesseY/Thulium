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
    STATIC_CLASS(L"/Script/Engine.World");

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
    static UWorld* GetWorld();

    static int64 GetNetMode(UWorld* This)
    {
        return 1;
    }

    static void Init()
    {
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

            Hook::Function(Addr, GetNetMode);
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
    }

public:
    void Listen();

};
