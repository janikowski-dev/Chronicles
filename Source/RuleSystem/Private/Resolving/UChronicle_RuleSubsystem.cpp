#include "UChronicle_RuleSubsystem.h"

#include "Data/EChronicle_ConditionNodeType.h"
#include "Data/FChronicle_CallbackData.h"
#include "Data/FChronicle_RuleData.h"

void UChronicle_RuleSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	EmptyRules();
}

void UChronicle_RuleSubsystem::Deinitialize()
{
	EmptyRules();
	Super::Deinitialize();
}

bool UChronicle_RuleSubsystem::Resolve(TArray<FChronicle_RuleData> Rules)
{
    if (Rules.IsEmpty())
    {
        return true;
    }

    TMap<FGuid, FChronicle_RuleData> RuleMap;
    
    for (const FChronicle_RuleData& Rule : Rules)
    {
        RuleMap.Add(Rule.Id, Rule);
    }

    const FChronicle_RuleData* OutputNode = Rules.FindByPredicate([](const FChronicle_RuleData& Rule)
    {
        return Rule.Type == EChronicle_ConditionNodeType::Output;
    });

    if (!OutputNode)
    {
        return false;
    }

    TFunction<bool(const FGuid&)> EvaluateNode = [&](const FGuid& NodeId) -> bool
    {
        const FChronicle_RuleData* Node = RuleMap.Find(NodeId);
        
        if (!Node)
        {
            return false;
        }

        switch (Node->Type)
        {
            case EChronicle_ConditionNodeType::Output:
            {
                if (Node->Input.IsEmpty())
                {
                    return true;
                }
                    
                return EvaluateNode(Node->Input[0]);
            }

            case EChronicle_ConditionNodeType::And:
            {
                for (const FGuid& InputId : Node->Input)
                {
                    if (!EvaluateNode(InputId))
                    {
                        return false;
                    }
                }
                    
                return true;
            }

            case EChronicle_ConditionNodeType::Or:
            {
                for (const FGuid& InputId : Node->Input)
                {
                    if (EvaluateNode(InputId))
                    {
                        return true;
                    }
                }
                    
                return false;
            }

            case EChronicle_ConditionNodeType::Not:
            {
                if (Node->Input.IsEmpty())
                {
                    return true;
                }
                    
                return !EvaluateNode(Node->Input[0]);
            }

            case EChronicle_ConditionNodeType::Raw:
            {
                switch (Node->ParameterType)
                {
                    case EChronicle_RuleParameterType::None:
                    {
                        const bool* Value = BoolRules.Find(Node->Id);
                        return Value ? *Value : false;
                    }
                    case EChronicle_RuleParameterType::Integer:
                    {
                        const int* Stored = IntegerRules.Find(Node->Id);
                        return Stored ? *Stored == Node->ParameterInteger : false;
                    }
                    case EChronicle_RuleParameterType::CharacterId:
                    {
                        const FGuid* Stored = CharacterIdRules.Find(Node->Id);
                        return Stored ? *Stored == Node->ParameterCharacterId : false;
                    }
                }
                return false;
            }
        }

        return false;
    };

    return EvaluateNode(OutputNode->Id);
}

void UChronicle_RuleSubsystem::Raise(TArray<FChronicle_CallbackData> Callbacks)
{
    for (const FChronicle_CallbackData& Callback : Callbacks)
    {
        switch (Callback.ParameterType)
        {
        case EChronicle_RuleParameterType::None:
            OnRawCallbackCalled.Broadcast(Callback.Id);
            break;
        case EChronicle_RuleParameterType::CharacterId:
            OnCharacterIdCallbackCalled.Broadcast(Callback.Id, Callback.ParameterCharacterId);
            break;
        case EChronicle_RuleParameterType::Integer:
            OnIntegerCallbackCalled.Broadcast(Callback.Id, Callback.ParameterInteger);
            break;
        }
    }
}

void UChronicle_RuleSubsystem::SetBool(const FGuid RuleId, const bool bValue)
{
	BoolRules[RuleId] = bValue;
}

void UChronicle_RuleSubsystem::SetInteger(const FGuid RuleId, const int Value)
{
	IntegerRules[RuleId] = Value;
}

void UChronicle_RuleSubsystem::SetCharacterId(const FGuid RuleId, const FGuid CharacterId)
{
	CharacterIdRules[RuleId] = CharacterId;
}

void UChronicle_RuleSubsystem::EmptyRules()
{
	BoolRules.Empty();
	CharacterIdRules.Empty();
	IntegerRules.Empty();
}
