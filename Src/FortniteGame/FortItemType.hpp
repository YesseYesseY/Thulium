#pragma once

struct EFortItemType
{
    STATIC_ENUM(L"/Script/FortniteGame.EFortItemType");

    ENUM_PROP(Ammo);
    ENUM_PROP(EditTool);
    ENUM_PROP(Trap);
    ENUM_PROP(WorldResource);
    ENUM_PROP(BuildingPiece);
    ENUM_PROP(WeaponHarvest);
};
