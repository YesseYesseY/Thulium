#pragma once
#include "Class.hpp"
#include "Function.hpp"
#include "../Hook.hpp"

void UClass::HookVTable(const std::string& Name, void* Hook, void** Original)
{
    auto Default = DefaultObject;
    if (!DefaultObject)
        return;

    void** Vtable = Default->VTable;
    if (!Vtable)
        return;

    auto Func = GetFunc(Name);
    if (!Func)
        return;

    auto Idx = Func->GetVTableIndex();
    if (Original)
        *Original = Vtable[Idx];
    Hook::Patch((uintptr_t)(&Vtable[Idx]), Hook);
}
