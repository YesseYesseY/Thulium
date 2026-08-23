#pragma once
#include "../Basic.hpp"

struct FGuid
{
    int32 A;
    int32 B;
    int32 C;
    int32 D;

    inline bool operator==(const FGuid& Other) const
    {
        return A == Other.A && B == Other.B && C == Other.C && D == Other.D;
    }
};
