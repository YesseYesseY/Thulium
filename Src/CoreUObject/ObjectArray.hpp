#pragma once
#include <fstream>

#include "Object.hpp"

struct FUObjectItem
{
    UObject* Object;
    int32 Flags;
    int32 ClusterRootIndex;
    int32 SerialNumber;
};

class FFixedUObjectArray
{
public:
    FUObjectItem* Objects;
    int32 MaxElements;
    int32 NumElements;

    inline FUObjectItem* GetItem(int32 Index) const
    {
        if (Index < 0 || Index >= NumElements)
            return nullptr;

        return &Objects[Index];
    }
};

class FChunkedFixedUObjectArray
{
    enum
    {
        NumElementsPerChunk = 64 * 1024,
    };

public:
    FUObjectItem** Objects;
    FUObjectItem* PreAllocatedObjects;
    int32 MaxElements;
    int32 NumElements;
    int32 MaxChunks;
    int32 NumChunks;

    inline bool IsValidIndex(int32 Index) const
    {
        return Index >= 0 && Index < NumElements;
    }

    inline FUObjectItem* GetItem(int32 Index) const
    {
        const int32 ChunkIndex = Index / NumElementsPerChunk;
        const int32 WithinChunkIndex = Index % NumElementsPerChunk;
        if (!IsValidIndex(Index) || ChunkIndex >= NumChunks || Index >= MaxElements)
            return nullptr;

        FUObjectItem* Chunk = Objects[ChunkIndex];
        if (!Chunk)
            return nullptr;

        return Chunk + WithinChunkIndex;
    }
};

class FUObjectArray
{
    union
    {
        FFixedUObjectArray FixedArray;
        FChunkedFixedUObjectArray ChunkedArray;
    };

public:
    inline int32 Num() const
    {
        return FixedArray.NumElements;
    }

    inline FUObjectItem* GetItem(int32 Index) const
    {
        return FixedArray.GetItem(Index);
    }

    inline UObject* Get(int32 Index) const
    {
        if (auto Item = FixedArray.GetItem(Index))
            return Item->Object;

        return nullptr;
    }

    inline void Dump(const char* FileName = "objects.txt") const
    {
        std::ofstream Out(FileName);
        for (int32 i = 0; i < Num(); i++)
        {
            if (auto Object = Get(i))
            {
                Out << Object->GetFullName() << '\n';
            }
        }
        Out.close();
    }
};
