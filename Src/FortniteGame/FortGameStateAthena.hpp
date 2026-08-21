#pragma once
#include "FortGameStatePvP.hpp"
#include "FortPlaylistAthena.hpp"

class AFortGameStateAthena : public AFortGameStatePvP
{
    STATIC_CLASS(L"/Script/FortniteGame.FortGameStateAthena");

    CLASS_PROP(int32, CurrentPlaylistId);
    CLASS_PROP(UFortPlaylistAthena*, CurrentPlaylistData);

    void OnRep_CurrentPlaylistData()
    {
        UFUNC("OnRep_CurrentPlaylistData");
        ProcessEvent(Func);
    }

    void OnRep_CurrentPlaylistId()
    {
        UFUNC("OnRep_CurrentPlaylistId");
        ProcessEvent(Func);
    }
};
