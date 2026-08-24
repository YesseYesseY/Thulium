#pragma once

enum class EFortStructuralGridQueryResults : uint8
{
    CanAdd = 0,
    ExistingActor = 1,
    Obstructed = 2,
    NoStructuralSupport = 3,
    InvalidActor = 4,
    ReachedLimit = 5,
    NoEditPermission = 6,
    PatternNotPermittedByLayoutRequirement = 7,
    ResourceTypeNotPermittedByLayoutRequirement = 8,
    BuildingAtRequirementsDisabled = 9,
    BuildingOtherThanRequirementsDisabled = 10
};
