#pragma once
#include "ReplicationDriver.hpp"
#include "URL.hpp"

class UWorld;

class UNetDriver : public UObject
{
    STATIC_CLASS(L"/Script/Engine.NetDriver");

    CLASS_PROP(FName, NetDriverName);
    CLASS_PROP(UReplicationDriver*, ReplicationDriver);

private:
    static inline void (*_SetWorld)(UNetDriver*, UWorld*) = nullptr;
    static inline bool (*_InitListen)(UNetDriver*, UObject*, FURL*, bool, FString& Error) = nullptr;
    static inline void (*_TickFlush)(UNetDriver*, float) = nullptr;

public:
    static void TickFlushHook(UNetDriver* This, float DeltaSeconds)
    {
        auto ReplicationDriver = This->ReplicationDriver;
        if (ReplicationDriver)
            ReplicationDriver->ServerReplicateActors(DeltaSeconds);

        return _TickFlush(This, DeltaSeconds);
    }

public:
    static void Init()
    {
        // SetWorld
        {
            auto Addr = Memcury::Scanner::FindStringRef(L"BeaconPort=").ScanForOpCode(0xE8, 1).RelativeOffset(1).Get();

            if (!Addr)
            {
                MsgBox("Failed to find UNetDriver::SetWorld");
                return;
            }

            _SetWorld = decltype(_SetWorld)(Addr);
        }

        // InitListen
        {
            auto Addr = Memcury::Scanner::FindStringRef(L"%s IpNetDriver listening on port %i").ScanFor({ 0x48, 0x89, 0x5C }, false, 1).Get();

            if (!Addr)
            {
                MsgBox("Failed to find UIpNetDriver::InitListen");
                return;
            }

            _InitListen = decltype(_InitListen)(Addr);
        }

        // TickFlush
        {
            auto Addr = Memcury::Scanner::FindStringRef(L"STAT_NetTickFlush").ScanFor({ 0x4C, 0x8B, 0xDC }, false).Get();

            if (!Addr)
            {
                MsgBox("Failed to find UNetDriver::TickFlush");
                return;
            }

            Hook::Function(Addr, TickFlushHook, &_TickFlush);
        }
    }

public:
    void SetWorld(UWorld* World)
    {
        _SetWorld(this, World);
    }

    bool InitListen(UObject* InNotify, FURL* URL, bool bReuseAddressAndPort, FString& Error)
    {
        return _InitListen(this, InNotify, URL, bReuseAddressAndPort, Error);
    }

    void TickFlush(float DeltaSeconds)
    {
        return _TickFlush(this, DeltaSeconds);
    }
};
