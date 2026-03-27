#pragma once

#include "FChronicle_SequenceInfo.h"
#include "FChronicle_DialogueInfo.generated.h"

USTRUCT(BlueprintType)
struct CINEMATICTIMELINE_API FChronicle_DialogueInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FGuid, FChronicle_SequenceInfo> SequencesById;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FGuid EntrySequenceId;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FGuid Id;
};