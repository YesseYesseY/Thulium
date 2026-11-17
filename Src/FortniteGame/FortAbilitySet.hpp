#pragma once
#include "../Engine/PrimaryDataAsset.hpp"
#include "../GameplayAbilities/AbilitySystemComponent.hpp"

class UFortAbilitySet : public UPrimaryDataAsset
{
    STATIC_CLASS(UFortAbilitySet);

    CLASS_PROP(TArray<UClass*>, GameplayAbilities);

    void Give(UAbilitySystemComponent* Component)
    {
        for (auto& Class : GameplayAbilities)
            Component->GiveAbility(Class);
    }
};
