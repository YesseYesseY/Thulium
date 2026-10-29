#include "Function.hpp"
#include "Class.hpp"

void UFunction::Init()
{
    _offset_FunctionFlags = UStruct::StaticClass()->Size;
    _offset_Func = 0xB0;
}
