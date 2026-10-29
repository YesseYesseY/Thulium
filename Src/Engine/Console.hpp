#pragma once
#include "ConsoleSettings.hpp"

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

class UConsole : public UObject
{
    STATIC_CLASS(L"/Script/Engine.Console");

    OFFSET_PROP(TArray<FAutoCompleteCommand>, AutoCompleteList);
    OFFSET_PROP(FAutoCompleteNode, AutoCompleteTree);

    void BuildRuntimeAutoCompleteList()
    {
        auto ConsoleSettings = UConsoleSettings::DefaultObj();
        auto CommandColor = ConsoleSettings->AutoCompleteCommandColor;

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
