#include "../CoreUObject.hpp"

FVector FRotator::Vector() const
{
    static auto Lib = UObject::FindObject(L"/Script/Engine.Default__KismetMathLibrary");
    static auto Func = UObject::FindFunction(L"/Script/Engine.KismetMathLibrary:Conv_RotatorToVector");
    struct
    {
        FRotator InRot;
        FVector Ret;
    } Args { *this };
    Lib->ProcessEvent(Func, &Args);
    return Args.Ret;
}
