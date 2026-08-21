#include "Struct.hpp"
#include "Function.hpp"
#include "Property.hpp"

UProperty* UStruct::GetProp(const std::string& Name)
{
    static auto FunctionClass = UFunction::StaticClass();

    for (UStruct* CurrentStruct = this; CurrentStruct; CurrentStruct = CurrentStruct->Super)
    {
        for (auto Child = CurrentStruct->Children; Child; Child = Child->Next)
        {
            if (!Child->IsA(FunctionClass) && Child->GetName() == Name)
            {
                return (UProperty*)Child;
            }
        }
    }

    return nullptr;
}

UFunction* UStruct::GetFunc(const std::string& Name)
{
    static auto FunctionClass = UFunction::StaticClass();

    for (UStruct* CurrentStruct = this; CurrentStruct; CurrentStruct = CurrentStruct->Super)
    {
        for (auto Child = CurrentStruct->Children; Child; Child = Child->Next)
        {
            if (Child->IsA(FunctionClass) && Child->GetName() == Name)
            {
                return (UFunction*)Child;
            }
        }
    }

    return nullptr;
}
