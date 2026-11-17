#pragma once
#include "MovementComponent.hpp"

class UProjectileMovementComponent : public UMovementComponent
{
    STATIC_CLASS(UProjectileMovementComponent);

    CLASS_PROP(float, MaxSpeed);
};
