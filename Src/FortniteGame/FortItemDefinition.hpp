#pragma once
#include "../Engine/PrimaryDataAsset.hpp"
#include "FortItem.hpp"
#include "FortItemType.hpp"

// Technically the base class is UMcpItemDefinitionBase
class UFortItemDefinition : public UPrimaryDataAsset
{
    STATIC_CLASS(L"/Script/FortniteGame.FortItemDefinition");

    CLASS_PROP(uint8, ItemType);

public:
    UFortItem* CreateTemporaryItemInstanceBP(int32 Count, int32 Level = 1)
    {
        UFUNC("CreateTemporaryItemInstanceBP");
        struct {
            int32 Count;
            int32 Level;
            UFortItem* Ret;
        } Args { Count, Level };
        ProcessEvent(Func, &Args);
        return Args.Ret;
    }

public:
    bool GoesInPrimaryQuickbar() const
    {
        static std::vector<int64> InvalidTypes = {
            EFortItemType::Ammo(), EFortItemType::EditTool(), EFortItemType::Trap(),
            EFortItemType::WorldResource(), EFortItemType::BuildingPiece()
        };

        auto CurrentType = ItemType;
        for (auto Type : InvalidTypes)
        {
            if (CurrentType == Type)
                return false;
        }

        return true;
    }
};
