#pragma once
#include "../Engine/FastArraySerializer.hpp"
#include "GameplayAbilitySpec.hpp"

struct FGameplayAbilitySpecContainer : public FFastArraySerializer
{
    STATIC_STRUCT(FGameplayAbilitySpecContainer);

    STRUCT_PROP(TArray<FGameplayAbilitySpec>, Items);
};
