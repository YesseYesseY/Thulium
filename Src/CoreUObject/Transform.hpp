#pragma once
#include "Vector.hpp"
#include "Quat.hpp"
#include "Rotator.hpp"

struct alignas(16) FTransform
{
    FQuat Rotation;
    FVector Translation;
    char pad1[4];
    FVector Scale3D;
    char pad2[4];

    FTransform()
    {
    }

    FTransform(const FVector& Location, const FRotator& Rotation, const FVector& Scale);
};
