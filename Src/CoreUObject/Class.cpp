#pragma once
#include "Class.hpp"
#include "ObjectArray.hpp"
#include "Function.hpp"
#include "../Hook.hpp"

void UClass::HookVTable(int32 Idx, void* Hook, void** Original)
{
    auto Default = DefaultObject;
    if (!DefaultObject)
        return;

    void** Vtable = Default->VTable;
    if (!Vtable)
        return;

    if (Original)
        *Original = Vtable[Idx];
    Hook::Patch((uintptr_t)(&Vtable[Idx]), Hook);
}

void UClass::HookVTable(const std::string& Name, void* Hook, void** Original)
{
    auto Func = GetFunc(Name);
    if (!Func)
        return;

    auto Idx = Func->GetVTableIndex();
    HookVTable(Idx, Hook, Original);
}

void UClass::HookAllVTables(int32 Idx, void* Hook)
{
    for (int32 i = 0; i < UObject::Objects->Num(); i++)
    {
        auto Object = UObject::Objects->Get(i);
        if (!Object || !Object->IsA(UClass::StaticClass()))
            continue;

        auto ObjClass = (UClass*)Object;
        if (!ObjClass->IsChildOf(this))
            continue;

        ObjClass->HookVTable(Idx, Hook);
    }
}
