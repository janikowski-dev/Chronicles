#pragma once

#include "EChronicle_RuleParameterType.h"
#include "UChronicle_RuleNode.h"
#include "UChronicle_RuleInputNode.generated.h"

UCLASS()
class DIALOGUESYSTEMEDITOR_API UChronicle_RuleInputNode : public UChronicle_RuleNode
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	EChronicle_RuleParameterType ParameterType;
	
	UPROPERTY()
	FGuid RuleId;
	
	UPROPERTY()
	FGuid CharacterIdParameter;
	
	UPROPERTY()
	int32 IntegerParameter;
};
