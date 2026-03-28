#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Graphs/UChronicle_DialogueGraph.h"
#include "Nodes/Unreal/UChronicle_DialogueNode.h"
#include "UChronicle_DialogueAsset.generated.h"

class UChronicle_RuleGraph;

UCLASS(BlueprintType, Blueprintable)
class DIALOGUESYSTEMEDITOR_API UChronicle_DialogueAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TMap<UChronicle_DialogueNode*, TObjectPtr<UChronicle_RuleGraph>> InnerGraphsByNode;
	
	UPROPERTY()
	TObjectPtr<UChronicle_DialogueGraph> Graph;
};