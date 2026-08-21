#pragma once
#include <MinHook.h>

namespace Hook
{
    template <typename T = void*>
    static void Function(uintptr_t Addr, void* Hook, T* Original = nullptr)
    {
        MH_CreateHook((LPVOID)Addr, Hook, (LPVOID*)Original);
        MH_EnableHook((LPVOID)Addr);
    }

    template <typename T>
    void Patch(uintptr_t addr, T byte)
    {
        DWORD yes;
        VirtualProtect((LPVOID)(addr), sizeof(T), PAGE_EXECUTE_READWRITE, &yes);
        *(T*)(addr) = byte;
        VirtualProtect((LPVOID)(addr), sizeof(T), yes, &yes);
    }

    static void ReturnHook()
    {
        return;
    }

    static bool ReturnTrue()
    {
        return true;
    }

    static bool ReturnFalse()
    {
        return false;
    }
}
