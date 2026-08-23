#pragma
#include "FortItem.hpp"
#include "FortItemEntry.hpp"

class UFortWorldItem : public UFortItem
{
    STATIC_CLASS(L"/Script/FortniteGame.FortWorldItem");

    CLASS_PROP(FFortItemEntry, ItemEntry);
};
