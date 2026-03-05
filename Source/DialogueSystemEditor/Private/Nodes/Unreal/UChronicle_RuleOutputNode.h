#pragma once

#include "UChronicle_RuleNode.h"
#include "UChronicle_RuleOutputNode.generated.h"

UENUM()
enum class EOutputType : int8
{
	PostCallback,
	Requirements
};

UCLASS()
class DIALOGUESYSTEMEDITOR_API UChronicle_RuleOutputNode : public UChronicle_RuleNode
{
	GENERATED_BODY()
	
public:
	virtual void AllocateDefaultPins() override;
	virtual bool AllowMultipleInputs() const override;
	virtual FText GetTitle() const override;
	virtual FSlateColor GetBackgroundColor() const override;
	virtual bool CanUserDeleteNode() const override;
	
public:
	UPROPERTY()
	EOutputType Type;
};
