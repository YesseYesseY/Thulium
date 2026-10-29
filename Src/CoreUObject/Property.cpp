#include "Property.hpp"
#include "Class.hpp"

FString UProperty::GetCPPTypeF()
{
    void (*func)(UProperty*, FString*, FString*, uint32) = decltype(func)(VTable[_offset_GetCPPType]);
    FString Ret;
    func(this, &Ret, nullptr, 0);
    return Ret;
}
