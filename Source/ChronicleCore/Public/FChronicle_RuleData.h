#pragma once

#include "EChronicle_RuleParameterType.h"
#include "EChronicle_ConditionNodeType.h"
#include "FChronicle_RuleData.generated.h"

USTRUCT()
struct CHRONICLECORE_API FChronicle_RuleData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	EChronicle_ConditionNodeType Type = EChronicle_ConditionNodeType::Raw;

	UPROPERTY(VisibleAnywhere)
	TArray<FGuid> Input;
	
	UPROPERTY(VisibleAnywhere)
	TArray<FGuid> Output;
	
	UPROPERTY(VisibleAnywhere)
	FGuid Id;
	
	UPROPERTY(VisibleAnywhere)
	EChronicle_RuleParameterType ParameterType;
	
	UPROPERTY(VisibleAnywhere)
	FGuid ParameterCharacterId;
	
	UPROPERTY(VisibleAnywhere)
	int32 ParameterInteger;
};