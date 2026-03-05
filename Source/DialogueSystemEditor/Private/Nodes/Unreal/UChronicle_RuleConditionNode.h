#pragma once

#include "UChronicle_RuleInputNode.h"
#include "UChronicle_RuleConditionNode.generated.h"

UCLASS()
class DIALOGUESYSTEMEDITOR_API UChronicle_RuleConditionNode : public UChronicle_RuleInputNode
{
	GENERATED_BODY()
	
public:
	virtual void AllocateDefaultPins() override;
	virtual FText GetTitle() const override;
	virtual FSlateColor GetBackgroundColor() const override;
};
