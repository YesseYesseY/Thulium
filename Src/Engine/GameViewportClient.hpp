#pragma once
#include "Console.hpp"
#include "World.hpp"

class UGameViewportClient : public UObject
{
    STATIC_CLASS(UGameViewportClient);

    CLASS_PROP(UConsole*, ViewportConsole);
    CLASS_PROP(UWorld*, World);
};
