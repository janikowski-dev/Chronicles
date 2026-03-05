#pragma once

#include "ERuleParameterType.h"
#include "EConditionNodeType.h"
#include "FRuleData.generated.h"

USTRUCT()
struct CHRONICLECORE_API FRuleData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	EConditionNodeType Type = EConditionNodeType::Raw;

	UPROPERTY(VisibleAnywhere)
	TArray<FGuid> Input;
	
	UPROPERTY(VisibleAnywhere)
	TArray<FGuid> Output;
	
	UPROPERTY(VisibleAnywhere)
	FGuid Id;
	
	UPROPERTY(VisibleAnywhere)
	ERuleParameterType ParameterType;
	
	UPROPERTY(VisibleAnywhere)
	FGuid ParameterCharacterId;
	
	UPROPERTY(VisibleAnywhere)
	int32 ParameterInteger;
};