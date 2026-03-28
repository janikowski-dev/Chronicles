#pragma once

#include "LevelSequence.h"
#include "Data/FChronicle_DialogueNodeData.h"
#include "FChronicle_SequenceData.generated.h"

USTRUCT(BlueprintType)
struct CINEMATICTIMELINEEDITOR_API FChronicle_SequenceData
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FGuid Id;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FChronicle_DialogueNodeData> Nodes;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<ULevelSequence> Sequence;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FGuid NextNodeId;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FGuid> BranchSequenceIds;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsEntrySequence;
};