#pragma once

#include "UChronicle_RuleNode.h"
#include "UChronicle_RuleAndNode.generated.h"

UCLASS()
class DIALOGUESYSTEMEDITOR_API UChronicle_RuleAndNode : public UChronicle_RuleNode
{
	GENERATED_BODY()
	
public:
	virtual void AllocateDefaultPins() override;
	virtual FText GetTitle() const override;
	virtual FSlateColor GetBackgroundColor() const override;
};
