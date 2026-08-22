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
        bool HasValidate = Memcury::Scanner::FindStringRef(GetNameW() + L"_Validate").IsValid();
        auto Scanner = Memcury::Scanner(Func);
        Scanner.ScanForOpCode(0xFF, HasValidate ? 1 : 0);

        return *Scanner.AbsoluteOffset(2).GetAs<int32*>() / 8;
    }
};
