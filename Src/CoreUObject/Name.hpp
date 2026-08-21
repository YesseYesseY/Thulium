#pragma once
#include "../Basic.hpp"

class FName
{
private:
    int64 pad;

public:
    FName() : pad(0) {}
    FName(const wchar_t* Str);

    std::string ToString() const;
};
