#pragma once
#include "Struct.hpp"

class UFunction : public UStruct
{
    STATIC_CLASS(L"/Script/CoreUObject.Function");

    OFFSET_PROP(void*, Func);

    static void Init()
    {
        _offset_Func = 0xB0;
    }

    int32 GetVTableIndex()
    {
        auto Scanner = Memcury::Scanner(Func);
        Scanner.ScanForOpCode(0xFF);

        return *Scanner.AbsoluteOffset(2).GetAs<int32*>() / 8;
    }
};
