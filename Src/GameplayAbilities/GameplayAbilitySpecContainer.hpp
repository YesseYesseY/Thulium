#pragma once
#include "../Engine/FastArraySerializer.hpp"
#include "GameplayAbilitySpec.hpp"

struct FGameplayAbilitySpecContainer : public FFastArraySerializer
{
    STATIC_STRUCT(FGameplayAbilitySpecContainer, L"/Script/GameplayAbilities.GameplayAbilitySpecContainer");

    STRUCT_PROP(TArray<FGameplayAbilitySpec>, Items);
};
