#pragma once

#include "FDialogueNodeData.h"
#include "UDialogueData.generated.h"

UCLASS()
class CHRONICLECORE_API UDialogueData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	TArray<FDialogueNodeData> Nodes;
	
	UPROPERTY(VisibleAnywhere)
	TArray<FGuid> ParticipantIds;
};
