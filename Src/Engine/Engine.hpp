#pragma once
#include "../CoreUObject/Object.hpp"
#include "GameViewportClient.hpp"

class UEngine : public UObject
{
    STATIC_CLASS(UEngine);

    CLASS_PROP(UGameViewportClient*, GameViewport);

private:

public:
    static UEngine* GetEngine()
    {
        static UEngine* Ret = nullptr;
        if (!Ret)
            Ret = UObject::FindFirstObjectOfClass<UEngine>();

        return Ret;
    }

    static void Init()
    {
    }

    UNetDriver* CreateNetDriver(UWorld* World, FName Def);
};
