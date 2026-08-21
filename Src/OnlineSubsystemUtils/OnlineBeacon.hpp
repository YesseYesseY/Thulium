#pragma once
#include "../Engine/Actor.hpp"
#include "../Engine/NetDriver.hpp"

class AOnlineBeacon : public AActor
{
    STATIC_CLASS(L"/Script/OnlineSubsystemUtils.OnlineBeacon");

    CLASS_PROP(UNetDriver*, NetDriver);

private:
    static inline void (*PauseBeaconRequests)(AOnlineBeacon*, bool) = nullptr;

public:
    static void Init()
    {
        // PauseBeaconRequests
        {
            auto Addr = Memcury::Scanner::FindPattern("40 ? 48 83 EC 30 48 8B ? 84 D2 74 ? 80 3D").Get();

            if (!Addr)
            {
                MsgBox("Failed to find PauseBeaconRequests");
                return;
            }

            PauseBeaconRequests = decltype(PauseBeaconRequests)(Addr);
        }
    }

public:
    void PauseRequests(bool Paused)
    {
        PauseBeaconRequests(this, Paused);
    }
};
