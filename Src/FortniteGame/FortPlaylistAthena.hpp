#pragma once
#include "FortPlaylist.hpp"

class UFortPlaylistAthena : public UFortPlaylist
{
    STATIC_CLASS(L"/Script/FortniteGame.FortPlaylistAthena");

    CLASS_PROP(int32, PlaylistId);
};
