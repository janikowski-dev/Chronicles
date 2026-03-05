#pragma once

#include "UChronicle_RuleNode.h"
#include "UChronicle_RuleNotNode.generated.h"

UCLASS()
class DIALOGUESYSTEMEDITOR_API UChronicle_RuleNotNode : public UChronicle_RuleNode
{
	GENERATED_BODY()
	
public:
	virtual void AllocateDefaultPins() override;
	virtual FText GetTitle() const override;
	virtual FSlateColor GetBackgroundColor() const override;
};
