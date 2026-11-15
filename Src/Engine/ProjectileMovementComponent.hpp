#pragma once
#include "MovementComponent.hpp"

class UProjectileMovementComponent : public UMovementComponent
{
    STATIC_CLASS(L"/Script/Engine.ProjectileMovementComponent");

    CLASS_PROP(float, MaxSpeed);
};
