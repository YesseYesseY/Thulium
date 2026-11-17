#pragma once

class UKismetMathLibrary : public UObject
{
    STATIC_CLASS(UKismetMathLibrary);

    static int32 RandomInteger(int32 Max)
    {
        static auto Func = UObject::FindFunction(L"/Script/Engine.KismetMathLibrary:RandomInteger");
        struct
        {
            int32 Max;
            int32 Ret;
        } Args { Max };
        DefaultObj()->ProcessEvent(Func, &Args);
        return Args.Ret;
    }

    static FVector RandomUnitVectorInEllipticalConeInRadians(const FVector& ConeDir, float MaxYawInRadians, float MaxPitchInRadians)
    {
        static auto Func = UObject::FindFunction(L"/Script/Engine.KismetMathLibrary:RandomUnitVectorInEllipticalConeInRadians");
        struct
        {
            FVector ConeDir;
            float MaxYawInRadians;
            float MaxPitchInRadians;
            FVector Ret;
        } Args { ConeDir, MaxYawInRadians, MaxPitchInRadians };
        DefaultObj()->ProcessEvent(Func, &Args);
        return Args.Ret;
    }

    static FVector TransformLocation(const FTransform& translivesmatter, const FVector& Location)
    {
        static auto Func = UObject::FindFunction(L"/Script/Engine.KismetMathLibrary:TransformLocation");
        struct
        {
            FTransform translivesmatter;
            FVector Location;
            FVector Ret;
        } Args { translivesmatter, Location };
        DefaultObj()->ProcessEvent(Func, &Args);
        return Args.Ret;
    }
};

