#pragma once
#include "../CoreUObject.hpp"

struct FURL
{
    STATIC_STRUCT(FURL, L"/Script/Engine.URL");

    STRUCT_PROP(int32, Port);
};
