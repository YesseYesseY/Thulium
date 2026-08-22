#pragma once
#include "Struct.hpp"

class UClass : public UStruct
{
    STATIC_CLASS(L"/Script/CoreUObject.Class");

    OFFSET_PROP(UObject*, DefaultObject);

    static void Init()
    {
        _offset_DefaultObject = 0xF8;
    }

    void HookVTable(int32 Idx, void* Hook, void** Original = nullptr);
    void HookVTable(const std::string& Name, void* Hook, void** Original = nullptr);

    template <typename T>
    void HookVTable(const std::string& Name, void* Hook, T* Original)
    {
        HookVTable(Name, Hook, (void**)Original);
    }
};
