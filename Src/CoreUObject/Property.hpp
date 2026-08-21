#pragma once
#include "Field.hpp"

class UProperty : public UField
{
    STATIC_CLASS(L"/Script/CoreUObject.Property");

    OFFSET_PROP(int32, Offset);

    static void Init()
    {
        _offset_Offset = 0x44;
    }
};
