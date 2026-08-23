#pragma once
#include "../Engine/Character.hpp"
#include "FortWeapon.hpp"
#include "FortWeaponItemDefinition.hpp"

class AFortPawn : public ACharacter
{
    STATIC_CLASS(L"/Script/FortniteGame.FortPawn");

    AFortWeapon* EquipWeaponDefinition(UFortWeaponItemDefinition* WeaponData, const FGuid& ItemEntryGuid)
    {
        UFUNC("EquipWeaponDefinition");

        struct {
            UFortWeaponItemDefinition* WeaponData;
            FGuid ItemEntryGuid;
            AFortWeapon* Ret;
        } Args { WeaponData, ItemEntryGuid };
        ProcessEvent(Func, &Args);
        return Args.Ret;
    }

public:
    AFortWeapon* EquipItemEntry(FFortItemEntry* ItemEntry)
    {
        return EquipWeaponDefinition((UFortWeaponItemDefinition*)ItemEntry->ItemDefinition, ItemEntry->ItemGuid);
    }
};
