#pragma once
#include "Field.hpp"

class UProperty;
class UFunction;

class UStruct : public UField
{
    STATIC_CLASS(L"/Script/CoreUObject.Struct");

    OFFSET_PROP(UStruct*, Super);
    OFFSET_PROP(UField*, Children);
    OFFSET_PROP(int32, Size);

public:
    static void Init()
    {
        _offset_Super = 0x30;
        _offset_Children = _offset_Super + 0x8;
        _offset_Size = _offset_Children + 0x8;
    }

public:
    bool IsChildOf(UStruct* Other) const
    {
        for (auto CurrentStruct = this; CurrentStruct; CurrentStruct = CurrentStruct->Super)
        {
            if (CurrentStruct == Other)
                return true;
        }

        return false;
    }

    UProperty* GetProp(const std::string& Name);
    UFunction* GetFunc(const std::string& Name);

    template <typename T>
    T* GetPropAs(const std::string& Name)
    {
        return (T*)GetProp(Name);
    }
};
