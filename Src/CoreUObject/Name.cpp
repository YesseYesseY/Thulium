#include "Name.hpp"
#include "Object.hpp"

FName::FName(const wchar_t* Str)
{
    static auto Lib = UObject::FindObject(L"/Script/Engine.Default__KismetStringLibrary");
    static auto Func = UObject::FindFunction(L"/Script/Engine.KismetStringLibrary:Conv_StringToName");

    struct {
        FString InStr;
        FName Ret;
    } Args { Str };
    Lib->ProcessEvent(Func, &Args);
    *this = Args.Ret;
}

FString FName::ToFString() const
{
    static auto Lib = UObject::FindObject(L"/Script/Engine.Default__KismetStringLibrary");
    static auto Func = UObject::FindFunction(L"/Script/Engine.KismetStringLibrary:Conv_NameToString");

    struct {
        FName InName;
        FString Ret;
    } Args { *this };
    Lib->ProcessEvent(Func, &Args);
    return Args.Ret;
}

std::string FName::ToString() const
{
    auto FStr = ToFString();
    auto Ret = FStr.ToString();
    FStr.Free();
    return Ret;
}

std::wstring FName::ToWString() const
{
    auto FStr = ToFString();
    auto Ret = FStr.ToWString();
    FStr.Free();
    return Ret;
}
