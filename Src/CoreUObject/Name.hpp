#pragma once
#include "../Basic.hpp"

class FName
{
private:
    int64 pad;

public:
    FName() : pad(0) {}
    FName(const wchar_t* Str);

    FString ToFString() const;
    std::string ToString() const;
    std::wstring ToWString() const;
};
