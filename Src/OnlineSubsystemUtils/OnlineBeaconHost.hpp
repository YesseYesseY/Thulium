#pragma once
#include "OnlineBeacon.hpp"

class AOnlineBeaconHost : public AOnlineBeacon
{
    STATIC_CLASS(L"/Script/OnlineSubsystemUtils.OnlineBeaconHost");

    CLASS_PROP(int32, ListenPort);

private:
    static inline bool (*_InitHost)(AOnlineBeaconHost*) = nullptr;

public:
    static void Init()
    {
        // InitHost
        {
            auto Addr = Memcury::Scanner::FindPattern("48 8B C4 48 81 EC C8 00 00 00 48 89 58 ? 4C 8D 05").Get();

            if (!Addr)
            {
                MsgBox("Failed to find AOnlineBeaconHost::InitHost");
                return;
            }

            _InitHost = decltype(_InitHost)(Addr);
        }
    }

public:
    bool InitHost()
    {
        return _InitHost(this);
    }
};
