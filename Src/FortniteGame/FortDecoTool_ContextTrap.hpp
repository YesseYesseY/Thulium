#pragma once
#include "FortTrapTool.hpp"

class AFortDecoTool_ContextTrap : public AFortTrapTool
{
    STATIC_CLASS(AFortDecoTool_ContextTrap);

    CLASS_PROP(UFortItemDefinition*, ContextTrapItemDefinition);
};
