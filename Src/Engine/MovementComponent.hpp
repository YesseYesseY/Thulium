#pragma once
#include "ActorComponent.hpp"

class UMovementComponent : public UActorComponent
{
    STATIC_CLASS(UMovementComponent);

    CLASS_PROP(FVector, Velocity);
};
