#pragma once

#include "EChronicle_TransitionType.h"
#include "Data/FChronicle_RuleData.h"
#include "Data/FChronicle_CallbackData.h"
#include "FChronicle_TransitionInfo.generated.h"

USTRUCT(BlueprintType)
struct CINEMATICTIMELINE_API FChronicle_TransitionInfo
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FChronicle_CallbackData> Callbacks;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FChronicle_RuleData> Rules;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EChronicle_TransitionType Type;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FGuid SequenceId;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FGuid NodeId;
};