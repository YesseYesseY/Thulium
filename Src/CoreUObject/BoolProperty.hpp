#pragma once
#include "Property.hpp"

class UBoolProperty : public UProperty
{
    STATIC_CLASS(L"/Script/CoreUObject.BoolProperty");

    OFFSET_PROP(uint8, FieldMask);

public:
    static void Init()
    {
        _offset_FieldMask = 0x73;
    }
};
