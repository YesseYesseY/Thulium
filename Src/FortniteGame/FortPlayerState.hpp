#pragma once
#include "../Engine/PlayerState.hpp"
#include "FortPawn.hpp"
#include "FortAbilitySystemComponent.hpp"

class AFortPlayerState : public APlayerState
{
    STATIC_CLASS(L"/Script/FortniteGame.FortPlayerState");

    CLASS_PROP(UFortAbilitySystemComponent*, AbilitySystemComponent);

private:
    static inline void (*_ApplyCustomizationToCharacter)(AFortPlayerState*, AFortPawn*) = nullptr;

public:
    void ApplyCustomizationToCharacter(AFortPawn* Pawn)
    {
        if (_ApplyCustomizationToCharacter)
            _ApplyCustomizationToCharacter(this, Pawn);
    }

public:
    static void Init()
    {
        // AFortPlayerState::ApplyCustomizationToCharacter
        {
            auto Addr = Memcury::Scanner::FindPattern("48 8B C4 48 89 50 ? 55 57 48 8D 68 ? 48 81 EC D8 00 00 00").Get();

            if (!Addr)
            {
                MsgBox("Failed to find AFortPlayerState::ApplyCustomizationToCharacter");
                return;
            }

            _ApplyCustomizationToCharacter = decltype(_ApplyCustomizationToCharacter)(Addr);
        }
    }
};
