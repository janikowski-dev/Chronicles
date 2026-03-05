#pragma once

#include "CoreMinimal.h"
#include "UChronicle_DialogueNode.generated.h"

class UChronicle_RuleGraph;

UCLASS()
class DIALOGUESYSTEMEDITOR_API UChronicle_DialogueNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
    virtual void PostPlacedNewNode() override;
	
	virtual FText GetTitle() const;
	virtual FText GetText() const;
	
	UChronicle_RuleGraph* GetOrCreateInnerGraph();
	UChronicle_RuleGraph* GetInnerGraph() const;

private:
	void AssignId();
	
public:
	UPROPERTY()
	bool bIsCollapsed = false;
	UPROPERTY()
	bool bIsHidden = false;
	UPROPERTY()
	FGuid Id;
};