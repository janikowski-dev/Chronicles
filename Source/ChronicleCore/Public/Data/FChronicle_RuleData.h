#pragma once

#include "EChronicle_RuleParameterType.h"
#include "EChronicle_ConditionNodeType.h"
#include "FChronicle_RuleData.generated.h"

USTRUCT(BlueprintType)
struct CHRONICLECORE_API FChronicle_RuleData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EChronicle_ConditionNodeType Type = EChronicle_ConditionNodeType::Raw;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FGuid> Input;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FGuid> Output;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FGuid Id;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EChronicle_RuleParameterType ParameterType = EChronicle_RuleParameterType::None;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FGuid ParameterCharacterId;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 ParameterInteger;
};