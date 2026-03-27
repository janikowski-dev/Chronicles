#pragma once

#include "FChronicle_DialogueNodeData.h"
#include "UChronicle_DialogueData.generated.h"

UCLASS()
class CHRONICLECORE_API UChronicle_DialogueData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	TArray<FChronicle_DialogueNodeData> Nodes;
	
	UPROPERTY(VisibleAnywhere)
	TArray<FGuid> ParticipantIds;
};
