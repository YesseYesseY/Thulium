#include "Object.hpp"
#include "Class.hpp"
#include "ObjectArray.hpp"

void UObject::Init()
{
    // StaticFindObject
    {
        auto Addr = Memcury::Scanner::FindStringRef(L"Illegal call to StaticFindObject() while serializing object data!").ScanFor({ 0x48, 0x89, 0x5C }, false).Get();

        if (!Addr)
        {
            MsgBox("Failed to find StaticFindObject");
            return;
        }

        _StaticFindObject = decltype(_StaticFindObject)(Addr);
    }

    // ProcessEvent
    {
        auto Addr = Memcury::Scanner::FindPattern("40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC F0 00 00 00").Get();

        if (!Addr)
        {
            MsgBox("Failed to find ProcessEvent");
            return;
        }

        _ProcessEvent = decltype(_ProcessEvent)(Addr);
    }

    // Objects
    {
        auto Addr = Memcury::Scanner::FindStringRef(L"Material=").ScanForEither({{ 0x48, 0x8B, 0x05 }, { 0x48, 0x8B, 0x0D }}).RelativeOffset(3).Get();

        if (!Addr)
        {
            MsgBox("Failed to find Objects");
            return;
        }

        Objects = (FUObjectArray*)(Addr);
    }
}

void UObject::ProcessEvent(UFunction* function, void* args)
{
    _ProcessEvent(this, function, args);
}

std::string UObject::GetName() const
{
    if (!this)
        return "None";

    return Name.ToString();
}

std::wstring UObject::GetNameW() const
{
    if (!this)
        return L"None";

    return Name.ToWString();
}

std::string UObject::GetPathName() const
{
    if (!this)
        return "None";

    static auto Lib = FindObject(L"/Script/Engine.Default__KismetSystemLibrary");
    static auto Func = Lib->Class->GetFunc("GetPathName");

    struct {
        const UObject* Object;
        FString Ret;
    } Args { this };
    Lib->ProcessEvent(Func, &Args);
    auto Ret = Args.Ret.ToString();
    Args.Ret.Free();
    return Ret;
}

std::string UObject::GetFullName() const
{
    if (!this || !Class)
        return "None";

    return std::format("{} {}", Class->GetName(), GetPathName());
}

bool UObject::IsA(UClass* Other) const
{
    return Class->IsChildOf(Other);
}

UObject* UObject::FindFirstObjectOfClass(UClass* Class)
{
    for (int32 i = 0; i < Objects->Num(); i++)
    {
        auto Object = Objects->Get(i);
        if (!Object)
            continue;

        if (Object->HasAnyFlags(EObjectFlags::ClassDefaultObject))
            continue;

        if (!Object->IsA(Class))
            continue;

        return Object;
    }

    return nullptr;
}
