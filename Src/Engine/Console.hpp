#pragma once
#include "ConsoleSettings.hpp"
#include "LevelScriptActor.hpp"

struct FAutoCompleteNode
{
    int32 IndexChar;
    TArray<int32> AutoCompleteListIndices;
    TArray<FAutoCompleteNode*> ChildNodes;

    FAutoCompleteNode()
    {
        IndexChar = -1;
    }

    FAutoCompleteNode(int32 NewChar)
    {
        IndexChar = NewChar;
    }
};

enum class EConsoleVariableFlags : uint32
{
    Default                  = 0x0,
    Cheat                    = 0x1,
    ReadOnly                 = 0x4,
    Unregistered             = 0x8,
    CreatedFromIni           = 0x10,
    RenderThreadSafe         = 0x20,
    Scalability              = 0x40,
    ScalabilityGroup         = 0x80,
    SetByMask                = 0xFF000000,
    SetByConstructor         = 0x00000000,
    SetByScalability         = 0x01000000,
    SetByGameSetting         = 0x02000000,
    SetByProjectSetting      = 0x03000000,
    SetBySystemSettingsIni   = 0x04000000,
    SetByDeviceProfile       = 0x05000000,
    SetByConsoleVariablesIni = 0x06000000,
    SetByCommandline         = 0x07000000,
    SetByCode                = 0x08000000,
    SetByConsole             = 0x09000000,
};
ENUM_CLASS_FLAGS(EConsoleVariableFlags);

class FConsoleVariableBase
{
public:
    void** VTable;
    FString Help;
    EConsoleVariableFlags Flags;

    bool HasFlag(EConsoleVariableFlags Other)
    {
        return (Flags & Other) != EConsoleVariableFlags::Default;
    }
};

class UConsole : public UObject
{
    STATIC_CLASS(L"/Script/Engine.Console");

    OFFSET_PROP(TArray<FAutoCompleteCommand>, AutoCompleteList);
    OFFSET_PROP(FAutoCompleteNode, AutoCompleteTree);

    void BuildRuntimeAutoCompleteList()
    {
        auto ConsoleSettings = UConsoleSettings::DefaultObj();
        auto CommandColor = ConsoleSettings->AutoCompleteCommandColor;
        auto CVarColor = ConsoleSettings->AutoCompleteCVarColor;
        auto FadedColor = ConsoleSettings->AutoCompleteFadedColor;

        // manual list
        {
            auto& ManList = ConsoleSettings->ManualAutoCompleteList;
            for (int32 Idx = 0; Idx < ManList.Num(); Idx++)
            {
                FAutoCompleteCommand ACC;
                ACC.Command = ManList[Idx].Command;
                ACC.Desc = ManList[Idx].Desc;
                ACC.Color = CommandColor;
                AutoCompleteList.Add(ACC);
            }
        }

        // console variables
        {
            auto IConsoleManager = *Memcury::Scanner::FindStringRef(L"gpucrashdebugging")
                .ScanForOpCode(0xE8, 2)
                .ScanForOpCode(0x8B)
                .RelativeOffset(3)
                .GetAs<uintptr_t*>();

            auto ConsoleObjects = *(TMap<FString, FConsoleVariableBase*>*)(IConsoleManager + 8);

            for (auto& thing : ConsoleObjects)
            {
                auto CVar = thing.Value();
                if (CVar->HasFlag(EConsoleVariableFlags::Unregistered))
                    continue;

                FAutoCompleteCommand ACC;
                ACC.Command = thing.Key();
                // ACC.Desc = CVar->Help;
                ACC.Color = CVar->HasFlag(EConsoleVariableFlags::ReadOnly) ? FadedColor : CVarColor;
                AutoCompleteList.Add(ACC);
            }
        }

        // exec functions
        for (int32 i = 0; i < UObject::Objects->Num(); i++)
        {
            auto Object = UObject::Objects->Get(i);
            if (!Object || !Object->IsA<UFunction>())
                continue;

            auto Func = (UFunction*)Object;
            auto FuncOuter = Func->Outer->Cast<UClass>();
            auto FuncSuperFunc = Func->Super->Cast<UFunction>();

            if (Func->HasAnyFunctionFlags(EFunctionFlags::Exec) && (FuncSuperFunc == nullptr || FuncOuter))
            {
                FAutoCompleteCommand ACC;

                auto FuncName = Func->GetNameW();
                for (int32 i = 0; i < FuncName.size(); i++)
                {
                    if (FuncName[i] == L' ')
                    {
                        FuncName = L'"' + FuncName + L'"';
                        break;
                    }
                }

                std::wstring Desc = L"";

                static auto FunctionClass = UFunction::StaticClass();
                for (auto Child = Func->Children; Child; Child = Child->Next)
                {
                    if (!Child->IsA(FunctionClass))
                    {
                        auto Prop = (UProperty*)Child;

                        if ((Prop->PropertyFlags & EPropertyFlags::Parm) != EPropertyFlags::None)
                            Desc += Prop->GetNameW() + L'[' + Prop->GetCPPTypeW() + L"] ";
                    }
                }

                ACC.Command = FuncName;
                ACC.Desc = Desc;
                ACC.Color = CommandColor;
                AutoCompleteList.Add(ACC);
            }
        }

        // misc
        {
            FAutoCompleteCommand ACCs[] = {
                { L"open 127.0.0.1", L"(opens connection to localhost)", CommandColor },
                { L"cheat devloadout", L"Gives you a lot of items", CommandColor },
                { L"cheat dumpobjects", L"Dump objects to objects.txt", CommandColor },
                { L"cheat startevent", L"Starts the event on event builds", CommandColor },
            };

            for (auto& ACC : ACCs)
                AutoCompleteList.Add(ACC);
        }

        auto& List = AutoCompleteList;
        for (int32 ListIdx = 0; ListIdx < List.Num(); ListIdx++)
        {
            auto Command = List[ListIdx].Command.ToLower();
            auto Node = &AutoCompleteTree;
            for (int32 Depth = 0; Depth < Command.Len(); Depth++)
            {
                int32 Char = Command[Depth];
                int32 FoundNodeIdx = -1;
                auto& NodeList = Node->ChildNodes;
                for (int32 NodeIdx = 0; NodeIdx < NodeList.Num(); NodeIdx++)
                {
                    if (NodeList[NodeIdx]->IndexChar == Char)
                    {
                        FoundNodeIdx = NodeIdx;
                        Node = NodeList[FoundNodeIdx];
                        NodeList[FoundNodeIdx]->AutoCompleteListIndices.Add(ListIdx);
                        break;
                    }
                }
                if (FoundNodeIdx == -1)
                {
                    auto NewNode = new FAutoCompleteNode(Char);
                    NewNode->AutoCompleteListIndices.Add(ListIdx);
                    Node->ChildNodes.Add(NewNode);
                    Node = NewNode;
                }
            }
            Command.Free();
        }
    }

    static void Init()
    {
        auto HBOffset = StaticClass()->GetProp("HistoryBuffer")->Offset;
        _offset_AutoCompleteList = HBOffset
            + sizeof(TArray<FString>)
            + sizeof(FString)
            + sizeof(int32) + 4
            + sizeof(FString)
            + sizeof(FString)
            + sizeof(uint32) + 4;
        _offset_AutoCompleteTree = _offset_AutoCompleteList
            + sizeof(TArray<FAutoCompleteCommand>)
            + sizeof(uint32) // Good one epic
            + sizeof(int32)
            + sizeof(int32)
            + sizeof(uint32) // Good one epic
            + sizeof(FName);
    }
};
