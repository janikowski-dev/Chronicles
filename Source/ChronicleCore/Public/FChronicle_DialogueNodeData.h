#pragma once

#include "EChronicle_DialogueNodeType.h"
#include "FChronicle_RuleData.h"
#include "FChronicle_DialogueNodeData.generated.h"

USTRUCT()
struct CHRONICLECORE_API FChronicle_DialogueNodeData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FGuid Id;

	UPROPERTY(VisibleAnywhere)
	EChronicle_DialogueNodeType Type;

	UPROPERTY(VisibleAnywhere)
	FString Text;

	UPROPERTY(VisibleAnywhere)
	FGuid SpeakerId;

	UPROPERTY(VisibleAnywhere)
	FGuid ListenerId;
	
	UPROPERTY(VisibleAnywhere)
	TArray<FChronicle_RuleData> Rules;

	UPROPERTY(VisibleAnywhere)
	TArray<FGuid> Callbacks;

	UPROPERTY(VisibleAnywhere)
	TArray<FGuid> Children;
};