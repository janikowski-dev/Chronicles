#pragma once

#include "EChronicle_DialogueNodeType.h"
#include "FChronicle_CallbackData.h"
#include "FChronicle_RuleData.h"
#include "UChronicle_EmotionAsset.h"
#include "FChronicle_DialogueNodeData.generated.h"

USTRUCT(BlueprintType)
struct CHRONICLECORE_API FChronicle_DialogueNodeData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FGuid Id;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EChronicle_DialogueNodeType Type = EChronicle_DialogueNodeType::Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UChronicle_EmotionAsset> Emotion;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString Subtitle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString Text;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FGuid SpeakerId;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FGuid ListenerId;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FChronicle_RuleData> Rules;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FChronicle_CallbackData> Callbacks;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FGuid> Children;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FGuid LinkTargetId;
};