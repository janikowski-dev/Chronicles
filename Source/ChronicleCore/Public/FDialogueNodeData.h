#pragma once

#include "EDialogueNodeType.h"
#include "FRuleData.h"
#include "FDialogueNodeData.generated.h"

USTRUCT()
struct CHRONICLECORE_API FDialogueNodeData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FGuid Id;

	UPROPERTY(VisibleAnywhere)
	EDialogueNodeType Type;

	UPROPERTY(VisibleAnywhere)
	FString Text;

	UPROPERTY(VisibleAnywhere)
	FGuid SpeakerId;

	UPROPERTY(VisibleAnywhere)
	FGuid ListenerId;
	
	UPROPERTY(VisibleAnywhere)
	TArray<FRuleData> Rules;

	UPROPERTY(VisibleAnywhere)
	TArray<FGuid> Callbacks;

	UPROPERTY(VisibleAnywhere)
	TArray<FGuid> Children;
};