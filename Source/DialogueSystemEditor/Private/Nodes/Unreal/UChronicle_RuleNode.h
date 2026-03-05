#pragma once

#include "CoreMinimal.h"
#include "UChronicle_RuleNode.generated.h"

UCLASS()
class DIALOGUESYSTEMEDITOR_API UChronicle_RuleNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	virtual void PostPlacedNewNode() override;
	virtual bool CanUserDeleteNode() const override;
	
	virtual bool AllowMultipleInputs() const;
	virtual FText GetTitle() const;
	virtual FSlateColor GetBackgroundColor() const;

private:
	void AssignId();

public:
	FGuid Id;
};