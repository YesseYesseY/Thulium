#pragma
#include "../Engine/FastArraySerializerItem.hpp"
#include "GameplayAbility.hpp"

struct FGameplayAbilitySpec : public FFastArraySerializerItem
{
    STATIC_STRUCT(FGameplayAbilitySpec, L"/Script/GameplayAbilities.GameplayAbilitySpec");

    STRUCT_PROP(FGameplayAbilitySpecHandle, Handle);
    STRUCT_PROP(UGameplayAbility*, Ability);
    STRUCT_BIT(InputPressed);
    STRUCT_PROP(int32, Level);
    STRUCT_PROP(int32, InputID);
};
