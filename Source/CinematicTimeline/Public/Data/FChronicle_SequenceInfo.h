#pragma once

#include "FChronicle_TransitionInfo.h"
#include "LevelSequence.h"
#include "FChronicle_SequenceInfo.generated.h"

USTRUCT(BlueprintType)
struct CINEMATICTIMELINE_API FChronicle_SequenceInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FGuid, FFrameNumber> StartFrameByNodeIds;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FChronicle_TransitionInfo> Transitions;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<ULevelSequence> Sequence;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsEntrySequence;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FGuid Id;
};