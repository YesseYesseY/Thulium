#pragma once

struct FGameplayAbilitySpecHandle
{
    int32 Handle;

    inline bool operator==(const FGameplayAbilitySpecHandle& Other) const
    {
        return Handle == Other.Handle;
    }
};
