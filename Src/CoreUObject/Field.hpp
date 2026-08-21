#pragma once
#include "Object.hpp"

class UField : public UObject
{
    STATIC_CLASS(L"/Script/CoreUObject.Field");

    UField* Next;
};
