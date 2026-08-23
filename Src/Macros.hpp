#define ENUM_CLASS_FLAGS(Enum) \
    inline constexpr Enum& operator|=(Enum& Lhs, Enum Rhs) { return Lhs = (Enum)((__underlying_type(Enum))Lhs | (__underlying_type(Enum))Rhs); } \
    inline constexpr Enum& operator&=(Enum& Lhs, Enum Rhs) { return Lhs = (Enum)((__underlying_type(Enum))Lhs & (__underlying_type(Enum))Rhs); } \
    inline constexpr Enum& operator^=(Enum& Lhs, Enum Rhs) { return Lhs = (Enum)((__underlying_type(Enum))Lhs ^ (__underlying_type(Enum))Rhs); } \
    inline constexpr Enum  operator| (Enum  Lhs, Enum Rhs) { return (Enum)((__underlying_type(Enum))Lhs | (__underlying_type(Enum))Rhs); } \
    inline constexpr Enum  operator& (Enum  Lhs, Enum Rhs) { return (Enum)((__underlying_type(Enum))Lhs & (__underlying_type(Enum))Rhs); } \
    inline constexpr Enum  operator^ (Enum  Lhs, Enum Rhs) { return (Enum)((__underlying_type(Enum))Lhs ^ (__underlying_type(Enum))Rhs); } \
    inline constexpr bool  operator! (Enum  E)             { return !(__underlying_type(Enum))E; } \
    inline constexpr Enum  operator~ (Enum  E)             { return (Enum)~(__underlying_type(Enum))E; }

#if CLIENT
#define MsgBoxTitle "ThuliumClient"
#else
#define MsgBoxTitle "ThuliumServer"
#endif
#define MsgBox(...) MessageBoxA(NULL, std::format(__VA_ARGS__).c_str(), MsgBoxTitle, 0)

#define COMMA ,

#define STATIC_CLASS(Name) \
public: \
    static UClass* StaticClass() \
    { \
        static UClass* Ret = UObject::FindClass(Name); \
        return Ret; \
    }

#define DEFAULT_OBJ(Name) \
    static Name* DefaultObj() \
    { \
        return (Name*)StaticClass()->DefaultObject; \
    }

#define STATIC_STRUCT(Type, Name) \
    static UStruct* StaticStruct() \
    { \
        static UStruct* Ret = UObject::FindStruct(Name); \
        return Ret; \
    } \
    static int32 Size() \
    { \
        return StaticStruct()->Size; \
    } \
    static Type* New() \
    { \
        auto Ret = FMemory::Malloc(Size()); \
        memset(Ret, 0, Size()); \
        return (Type*)Ret; \
    }



#define OFFSET_PROP(Type, Name) \
private: \
    static inline int32 _offset_##Name = -1; \
public: \
    Type& _get_##Name() const { return *(Type*)(int64(this) + _offset_##Name); } \
    void _put_##Name(Type val) { *(Type*)(int64(this) + _offset_##Name) = val; } \
    __declspec(property(get = _get_##Name, put = _put_##Name)) Type Name;



#define CLASS_PROP(Type, Name) \
public: \
    inline Type& _get_##Name() const { static int32 offset = Class->GetProp(#Name)->Offset; return *(Type*)(int64(this) + offset); } \
    inline void _put_##Name(Type val) { static int32 offset = Class->GetProp(#Name)->Offset; *(Type*)(int64(this) + offset) = val; } \
    __declspec(property(get = _get_##Name, put = _put_##Name)) Type Name;

#define CLASS_BIT(Name) \
public: \
    inline bool _get_##Name() const { static int32 offset = -1; static uint8 fieldmask = 0; \
        if (offset == -1) { auto prop = Class->GetPropAs<UBoolProperty>(#Name); offset = prop->Offset; fieldmask = prop->FieldMask; } \
        return (*(uint8*)(int64(this) + offset) & fieldmask) != 0; } \
    inline void _put_##Name(bool val) const { static int32 offset = -1; static uint8 fieldmask = 0; \
        if (offset == -1) { auto prop = Class->GetPropAs<UBoolProperty>(#Name); offset = prop->Offset; fieldmask = prop->FieldMask; } \
        if (val) *(uint8*)(int64(this) + offset) |= fieldmask; else *(uint8*)(int64(this) + offset) &= ~fieldmask; } \
    __declspec(property(get = _get_##Name, put = _put_##Name)) bool Name;



#define STRUCT_PROP(Type, Name) \
public: \
    inline Type& _get_##Name() const { static int32 offset = StaticStruct()->GetProp(#Name)->Offset; return *(Type*)(int64(this) + offset); } \
    inline void _put_##Name(Type val) { static int32 offset = StaticStruct()->GetProp(#Name)->Offset; *(Type*)(int64(this) + offset) = val; } \
    __declspec(property(get = _get_##Name, put = _put_##Name)) Type Name;

#define STRUCT_BIT(Name) \
public: \
    inline bool _get_##Name() const { static int32 offset = -1; static uint8 fieldmask = 0; \
        if (offset == -1) { auto prop = StaticStruct()->GetPropAs<UBoolProperty>(#Name); offset = prop->Offset; fieldmask = prop->FieldMask; } \
        return (*(uint8*)(int64(this) + offset) & fieldmask) != 0; } \
    inline void _put_##Name(bool val) const { static int32 offset = -1; static uint8 fieldmask = 0; \
        if (offset == -1) { auto prop = StaticStruct()->GetPropAs<UBoolProperty>(#Name); offset = prop->Offset; fieldmask = prop->FieldMask; } \
        if (val) *(uint8*)(int64(this) + offset) |= fieldmask; else *(uint8*)(int64(this) + offset) &= ~fieldmask; } \
    __declspec(property(get = _get_##Name, put = _put_##Name)) bool Name;



#define STATIC_ENUM(Name) \
public: \
    static UEnum* StaticEnum() \
    { \
        static UEnum* Ret = UObject::FindEnum(Name); \
        return Ret; \
    }

#define ENUM_PROP(Name) \
    static inline int64 Name() \
    { \
        static int64 Ret = StaticEnum()->GetValue(#Name); \
        return Ret; \
    } \



#define UFUNC(Name) static auto Func = Class->GetFunc(Name)
#define UFUNC_STATIC(Name) static auto Func = StaticClass()->GetFunc(Name)
