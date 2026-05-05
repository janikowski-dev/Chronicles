#pragma once

#include "EChronicle_RuleParameterType.h"
#include "FChronicle_CallbackData.generated.h"

USTRUCT(BlueprintType)
struct CHRONICLECORE_API FChronicle_CallbackData
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FGuid Id;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EChronicle_RuleParameterType ParameterType = EChronicle_RuleParameterType::None;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FGuid ParameterCharacterId;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 ParameterInteger;
};