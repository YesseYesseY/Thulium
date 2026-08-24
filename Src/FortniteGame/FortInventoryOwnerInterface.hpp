#pragma once
#include "FortPlayerControllerAthena.hpp"
#include "FortPlayerPawnAthena.hpp"

class UFortInventoryOwnerInterface
{
    STATIC_CLASS(L"/Script/FortniteGame.FortInventoryOwnerInterface");

private:
    static inline int32 ControllerOffset = -1;

private:
    static bool RemoveItem(uintptr_t This, const FGuid& ItemGuid, int32 Count, bool bForceRemoveFromQuickBars, bool bForceRemoval)
    {
        auto Controller = (AFortPlayerControllerAthena*)(This - ControllerOffset);
        return Controller->WorldInventory->RemoveItem(ItemGuid, Count);
    }

public:
    static void Init()
    {
        auto Default = AFortPlayerControllerAthena::StaticClass()->DefaultObject;
        auto VTable = Default->GetInterfaceAddress(UFortInventoryOwnerInterface::StaticClass());
        ControllerOffset = (uintptr_t)VTable - (uintptr_t)Default;
        Hook::Patch(*(uintptr_t*)VTable + 0xA0, (uintptr_t)RemoveItem);
    }
};
