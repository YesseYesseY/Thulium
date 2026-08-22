#pragma once
#include "../GameplayTasks/GameplayTasksComponent.hpp"
#include "GameplayAbilitySpecHandle.hpp"
#include "GameplayAbility.hpp"
#include "GameplayAbilitySpecContainer.hpp"
#include "PredictionKey.hpp"

class UAbilitySystemComponent : public UGameplayTasksComponent
{
    STATIC_CLASS(L"/Script/GameplayAbilities.AbilitySystemComponent");

    CLASS_PROP(FGameplayAbilitySpecContainer, ActivatableAbilities);

private:
    static inline FGameplayAbilitySpecHandle& (*_GiveAbility)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle&, FGameplayAbilitySpec*) = nullptr;
    static inline bool (*_InternalTryActivateAbility)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle, FPredictionKey*, UGameplayAbility**, void*, void*) = nullptr;

public:
    FGameplayAbilitySpecHandle GiveAbility(FGameplayAbilitySpec* Spec)
    {
        FGameplayAbilitySpecHandle Ret;
        _GiveAbility(this, Ret, Spec);
        return Ret;
    }

    bool InternalTryActivateAbility(FGameplayAbilitySpecHandle Handle, FPredictionKey* Key, UGameplayAbility** a3, void* a4, void* a5)
    {
        return _InternalTryActivateAbility(this, Handle, Key, a3, a4, a5);
    }

public:
    void GiveAbility(UClass* AbilityClass)
    {
        auto Spec = FGameplayAbilitySpec::New();
        Spec->ReplicationID = -1;
        Spec->ReplicationKey = -1;
        Spec->MostRecentArrayReplicationKey = -1;
        Spec->Handle.Handle = rand();
        Spec->Ability = (UGameplayAbility*)AbilityClass->DefaultObject;
        Spec->Level = 1;
        Spec->InputID = -1;
        GiveAbility(Spec);
    }

    FGameplayAbilitySpec* FindAbilitySpecFromHandle(FGameplayAbilitySpecHandle Handle)
    {
        auto& Items = ActivatableAbilities.Items;
        for (int i = 0; i < Items.Num(); i++)
        {
            auto& Spec = Items.Get(i, FGameplayAbilitySpec::Size());
            if (Spec.Handle == Handle)
            {
                return &Spec;
            }
        }

        return nullptr;
    }

public:
    void ClientActivateAbilityFailed(const FGameplayAbilitySpecHandle& AbilityToActivate, int16 PredictionKey)
    {
        UFUNC("ClientActivateAbilityFailed");
        struct {
            FGameplayAbilitySpecHandle AbilityToActivate;
            int16 PredictionKey;
        } Args { AbilityToActivate, PredictionKey };
        ProcessEvent(Func, &Args);
    }

public:
    static void InternalServerTryActivateAbilityHook(UAbilitySystemComponent* This, FGameplayAbilitySpecHandle Handle, bool InputPressed, FPredictionKey* PredictionKey, void* TriggerEventData)
    {
        auto Spec = This->FindAbilitySpecFromHandle(Handle);
        if (!Spec)
        {
            This->ClientActivateAbilityFailed(Handle, PredictionKey->Current);
            return;
        }

        auto AbilityToActivate = Spec->Ability;
        if (!AbilityToActivate)
        {
            This->ClientActivateAbilityFailed(Handle, PredictionKey->Current);
            return;
        }

        UGameplayAbility* InstancedAbility = nullptr;
        Spec->InputPressed = true;
        if (This->InternalTryActivateAbility(Handle, PredictionKey, &InstancedAbility, nullptr, TriggerEventData))
        {
        }
        else
        {
            This->ClientActivateAbilityFailed(Handle, PredictionKey->Current);
            Spec->InputPressed = true;

            This->ActivatableAbilities.MarkArrayDirty();
        }
    }

public:
    static void Init()
    {
        // GiveAbility && InternalTryActivateAbility
        {
            auto Scanner = Memcury::Scanner::FindStringRef(L"GiveAbilityAndActivateOnce called on ability %s on the client, not allowed!");

            Scanner.ScanForOpCode(0xE8, 2);
            _GiveAbility = decltype(_GiveAbility)(Memcury::PE::Address(Scanner.Get()).RelativeOffset(1).Get());

            Scanner.ScanForOpCode(0xE8, 1);
            _InternalTryActivateAbility = decltype(_InternalTryActivateAbility)(Memcury::PE::Address(Scanner.Get()).RelativeOffset(1).Get());
        }

        // InternalServerTryActivateAbility
        {
            auto Func = UObject::FindFunction(L"/Script/GameplayAbilities.AbilitySystemComponent:ServerTryActivateAbility");
            auto Idx = Func->GetVTableIndex();

            auto Vtable = StaticClass()->DefaultObject->VTable;
            auto Idx2 = *Memcury::Scanner(Vtable[Idx]).ScanForOpCode(0xFF).AbsoluteOffset(2).GetAs<int32*>() / 8;
            // TODO Hook all AbilitySystemComponent
            UObject::FindClass(L"/Script/FortniteGame.FortAbilitySystemComponentAthena")->HookVTable(Idx2, InternalServerTryActivateAbilityHook);
        }
    }
};
