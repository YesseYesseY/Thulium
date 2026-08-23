#pragma once
#include "Name.hpp"
#include "../Memcury.hpp"

enum class EObjectFlags : uint32
{
    NoFlags                      = 0x00000000,
    Public                       = 0x00000001,
    Standalone                   = 0x00000002,
    MarkAsNative                 = 0x00000004,
    Transactional                = 0x00000008,
    ClassDefaultObject           = 0x00000010,
    ArchetypeObject              = 0x00000020,
    Transient                    = 0x00000040,
    MarkAsRootSet                = 0x00000080,
    TagGarbageTemp               = 0x00000100,
    NeedInitialization           = 0x00000200,
    NeedLoad                     = 0x00000400,
    KeepForCooker                = 0x00000800,
    NeedPostLoad                 = 0x00001000,
    NeedPostLoadSubobjects       = 0x00002000,
    NewerVersionExists           = 0x00004000,
    BeginDestroyed               = 0x00008000,
    FinishDestroyed              = 0x00010000,
    BeingRegenerated             = 0x00020000,
    DefaultSubObject             = 0x00040000,
    WasLoaded                    = 0x00080000,
    TextExportTransient          = 0x00100000,
    LoadCompleted                = 0x00200000,
    InheritableComponentTemplate = 0x00400000,
    DuplicateTransient           = 0x00800000,
    StrongRefOnFrame             = 0x01000000,
    NonPIEDuplicateTransient     = 0x02000000,
    Dynamic                      = 0x04000000,
    WillBeLoaded                 = 0x08000000,
    HasExternalPackage           = 0x10000000,
    PendingKill                  = 0x20000000,
    Garbage                      = 0x40000000,
    AllocatedInSharedPage        = 0x80000000,
};
ENUM_CLASS_FLAGS(EObjectFlags);

class UStruct;
class UClass;
class UFunction;
class UEnum;

class UObject
{
private:
    static inline void* (*_StaticFindObject)(void*, void*, const wchar_t*, bool) = nullptr;
    static inline void (*_ProcessEvent)(void*, void*, void*) = nullptr;

public:
    static inline class FUObjectArray* Objects = nullptr;

public:
    void** VTable;
    EObjectFlags Flags;
    int32 Index;
    UClass* Class;
    FName Name;
    UObject* Outer;

    static void Init();

    template <typename T = UObject>
    static T* FindObject(const wchar_t* Name)
    {
        return (T*)_StaticFindObject(nullptr, nullptr, Name, false);
    }

    static UClass* FindClass(const wchar_t* Name)
    {
        static auto UClassClass = FindObject(L"/Script/CoreUObject.Class");
        return (UClass*)_StaticFindObject(UClassClass, nullptr, Name, false);
    }

    static UStruct* FindStruct(const wchar_t* Name)
    {
        static auto UScriptStructClass = FindClass(L"/Script/CoreUObject.ScriptStruct");
        return (UStruct*)_StaticFindObject(UScriptStructClass, nullptr, Name, false);
    }

    static UFunction* FindFunction(const wchar_t* Name)
    {
        static auto UFunctionClass = FindObject(L"/Script/CoreUObject.Function");
        return (UFunction*)_StaticFindObject(UFunctionClass, nullptr, Name, false);
    }

    static UEnum* FindEnum(const wchar_t* Name)
    {
        static auto UEnumClass = FindObject(L"/Script/CoreUObject.Enum");
        return (UEnum*)_StaticFindObject(UEnumClass, nullptr, Name, false);
    }

    void ProcessEvent(UFunction* function, void* args = nullptr);
    std::string GetName() const;
    std::wstring GetNameW() const;
    std::string GetPathName() const;
    std::string GetFullName() const;
    bool IsA(UClass* Other) const;

    template <typename T>
    bool IsA() const
    {
        return IsA(T::StaticClass());
    }

    inline bool HasAnyFlags(EObjectFlags OtherFlags) const
    {
        return (Flags & OtherFlags) != EObjectFlags::NoFlags;
    }

    static UObject* FindFirstObjectOfClass(UClass* Class);

    template <typename T>
    static inline T* FindFirstObjectOfClass()
    {
        return (T*)FindFirstObjectOfClass(T::StaticClass());
    }
};
