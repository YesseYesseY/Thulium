#pragma once
#include "../Engine/PrimaryDataAsset.hpp"
#include "../GameplayAbilities/AbilitySystemComponent.hpp"

class UFortAbilitySet : public UPrimaryDataAsset
{
    STATIC_CLASS(L"/Script/FortniteGame.FortAbilitySet");

    CLASS_PROP(TArray<UClass*>, GameplayAbilities);

    void Give(UAbilitySystemComponent* Component)
    {
        for (auto& Class : GameplayAbilities)
            Component->GiveAbility(Class);
    }
};
