#pragma once
#include "../Engine/PlayerController.hpp"
#include "FortInventory.hpp"

class AFortPlayerController : public APlayerController
{
    STATIC_CLASS(L"/Script/FortniteGame.FortPlayerController");

    CLASS_PROP(AFortInventory*, WorldInventory);

public:
    void ActivateSlot(uint8 InQuickBar, int32 Slot, float ActivateDelay, bool bUpdatePreviousFocusedSlot)
    {
        UFUNC("ActivateSlot");
        struct {
            uint8 InQuickBar;
            int32 Slot;
            float ActivateDelay;
            bool bUpdatePreviousFocusedSlot;
        } Args { InQuickBar, Slot, ActivateDelay, bUpdatePreviousFocusedSlot };
        ProcessEvent(Func, &Args);
    }
};
