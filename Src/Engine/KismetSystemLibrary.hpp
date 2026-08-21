#pragma once
#include "World.hpp"

class UKismetSystemLibrary : public UObject
{
    STATIC_CLASS(L"/Script/Engine.KismetSystemLibrary");

    static UKismetSystemLibrary* DefaultObj()
    {
        static auto Ret = StaticClass()->DefaultObject;
        return (UKismetSystemLibrary*)Ret;
    }

    static void ExecuteConsoleCommand(const wchar_t* Cmd)
    {
        static auto Func = UObject::FindFunction(L"/Script/Engine.KismetSystemLibrary:ExecuteConsoleCommand");
        struct
        {
            UObject* WorldContext;
            FString Command;
            UObject* Player; // TODO
        } Args { UWorld::GetWorld(), Cmd, nullptr };
        DefaultObj()->ProcessEvent(Func, &Args);
    }

    static std::string GetEngineVersion()
    {
        static auto Func = UObject::FindFunction(L"/Script/Engine.KismetSystemLibrary:GetEngineVersion");
        FString Ver;
        DefaultObj()->ProcessEvent(Func, &Ver);
        auto Ret = Ver.ToString();
        Ver.Free();
        return Ret;
    }
};

