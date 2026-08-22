#pragma once

struct FFastArraySerializer
{
    STATIC_STRUCT(FFastArraySerializer, L"/Script/Engine.FastArraySerializer");

    TMap<int32, int32> ItemMap;
    int32 IDCounter;
    int32 ArrayReplicationKey;
    TMap<int32, void*> GuidReferencesMap;
    int32 CachedNumItems;
    int32 CachedNumItemsToConsiderForWriting;

    void IncrementArrayReplicationKey()
    {
        ArrayReplicationKey++;
        if (ArrayReplicationKey == -1)
            ArrayReplicationKey++;
    }

    void MarkArrayDirty()
    {
        // ItemMap.Reset();
        IncrementArrayReplicationKey();

        CachedNumItems = -1;
        CachedNumItemsToConsiderForWriting = -1;
    }
};
