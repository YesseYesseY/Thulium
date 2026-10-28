#pragma once
#include "FortTrapTool.hpp"

class AFortDecoTool_ContextTrap : public AFortTrapTool
{
    STATIC_CLASS(L"/Script/FortniteGame.FortDecoTool_ContextTrap");

    CLASS_PROP(UFortItemDefinition*, ContextTrapItemDefinition);
};
