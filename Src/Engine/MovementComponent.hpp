#pragma once
#include "ActorComponent.hpp"

class UMovementComponent : public UActorComponent
{
    STATIC_CLASS(L"/Script/Engine.MovementComponent");

    CLASS_PROP(FVector, Velocity);
};
