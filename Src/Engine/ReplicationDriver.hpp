#pragma once
#include "../CoreUObject.hpp"

class UReplicationDriver : public UObject
{
    STATIC_CLASS(L"/Script/Engine.ReplicationDriver");

private:
    static inline void (*_ServerReplicateActors)(UReplicationDriver*, float) = nullptr;

public:
    static void Init()
    {
        // UReplicationGraph::ServerReplicateActors
        {
            auto RepDriverVTable = UObject::FindObject(L"/Script/Engine.Default__ReplicationDriver")->VTable;
            auto RepGraphVTable = UObject::FindObject(L"/Script/ReplicationGraph.Default__ReplicationGraph")->VTable;

            auto DriverFunc = Memcury::Scanner::FindStringRef(L"UReplicationDriver::ServerReplicateActors").ScanFor({ 0x40, 0x53 }, false).GetAs<void*>();

            if (RepDriverVTable && RepGraphVTable && DriverFunc)
            {
                for (int i = 0; i < 0x100; i++)
                {
                    if (RepDriverVTable[i] == DriverFunc)
                    {
                        _ServerReplicateActors = decltype(_ServerReplicateActors)(RepGraphVTable[i]);
                        break;
                    }
                }
            }
        }
    }

public:
    void ServerReplicateActors(float DeltaSeconds)
    {
        _ServerReplicateActors(this, DeltaSeconds);
    }
};
