#pragma once
#include "../CoreUObject/Class.hpp"
#include "Actor.hpp"
#include "World.hpp"

class UGameplayStatics : public UObject
{
    STATIC_CLASS(L"/Script/Engine.GameplayStatics");

    static UGameplayStatics* DefaultObj()
    {
        static auto Ret = StaticClass()->DefaultObject;
        return (UGameplayStatics*)Ret;
    }

    static UObject* SpawnObject(UClass* Class, UObject* Outer)
    {
        static auto Func = UObject::FindFunction(L"/Script/Engine.GameplayStatics:SpawnObject");
        struct
        {
            UClass* ObjectClass;
            UObject* OuterObj;
            UObject* Ret;
        } Args { Class, Outer };
        DefaultObj()->ProcessEvent(Func, &Args);
        return Args.Ret;
    }

    template <typename T = AActor>
    static TArray<T*> GetAllActorsOfClass(UClass* ActorClass = T::StaticClass())
    {
        static auto Func = UObject::FindFunction(L"/Script/Engine.GameplayStatics:GetAllActorsOfClass");
        struct {
            UObject* WorldContext;
            UClass* ActorClass;
            TArray<T*> Ret;
        } Args { UWorld::GetWorld(), ActorClass };
        DefaultObj()->ProcessEvent(Func, &Args);
        return Args.Ret;
    }

    template <typename T = AActor>
    static int32 GetNumActorsOfClass(UClass* ActorClass = T::StaticClass())
    {
        auto Actors = GetAllActorsOfClass<T>();
        auto Ret = Actors.Num();
        Actors.Free();
        return Ret;
    }

    template <typename T>
    static T* SpawnObject(UObject* Outer)
    {
        return (T*)SpawnObject(T::StaticClass(), Outer);
    }
};

