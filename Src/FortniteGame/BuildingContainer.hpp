#pragma once
#include "BuildingTimeOfDayLights.hpp"
#include "../Hook.hpp"

class ABuildingContainer : public ABuildingTimeOfDayLights
{
    STATIC_CLASS(ABuildingContainer);

    CLASS_PROP(float, LootTossConeHalfAngle_Athena);
    CLASS_PROP(float, LootTossSpeed_Athena);
    CLASS_PROP(FRotator, LootTossDirection_Athena);
    CLASS_PROP(FVector, LootSpawnLocation_Athena);

public:
    static void SpawnLootHook(ABuildingContainer* This);

public:
    static void Init()
    {
        // TODO
        // auto Idx = 0xD40 / 8;
        // StaticClass()->HookAllVTables(Idx, SpawnLootHook);

        auto Addr = Memcury::Scanner::FindPattern("48 83 EC 28 80 B9 ? ? ? ? ? 72").Get();

        if (!Addr)
        {
            MsgBox("Failed to find ABuildingContainer::SpawnLoot");
            return;
        }

        Hook::Function(Addr, SpawnLootHook);
    }
};
