#pragma once
#include "Field.hpp"

class UEnum : public UField
{
    STATIC_CLASS(L"/Script/CoreUObject.Enum");

    OFFSET_PROP(TArray<TPair<FName COMMA int64>>, Names);

    inline int64 GetValue(const std::string& Name) const
    {
        for (auto& thing : Names)
        {
            auto Key = thing.Key().ToString();
            auto Find = Key.find("::");
            if (Find != std::string::npos)
                Key = Key.substr(Find + 2);

            if (Key == Name)
                return thing.Value();
        }

        return -1;
    }

public:
    static void Init()
    {
        _offset_Names = 0x40;
    }
};
