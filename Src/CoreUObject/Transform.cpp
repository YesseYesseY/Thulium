#include "../CoreUObject.hpp"

FTransform::FTransform(const FVector& Location, const FRotator& Rotation, const FVector& Scale)
{
    static auto Lib = UObject::FindObject(L"/Script/Engine.Default__KismetMathLibrary");
    static auto Func = UObject::FindFunction(L"/Script/Engine.KismetMathLibrary:MakeTransform");
    struct {
        FVector Location;
        FRotator Rotation;
        FVector Scale;
        FTransform Ret;
    } Args { Location, Rotation, Scale };
    Lib->ProcessEvent(Func, &Args);
    *this = Args.Ret;
}
