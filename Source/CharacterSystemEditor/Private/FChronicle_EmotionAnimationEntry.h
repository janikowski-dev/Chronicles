#pragma once

#include "Data/UChronicle_EmotionAsset.h"
#include "FChronicle_EmotionAnimationEntry.generated.h"

USTRUCT()
struct FChronicle_EmotionAnimationEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UChronicle_EmotionAsset> Emotion;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UAnimSequence>> Sequences;
};