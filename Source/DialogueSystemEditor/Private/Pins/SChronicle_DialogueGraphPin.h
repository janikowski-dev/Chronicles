#pragma once

#include "SGraphPin.h"

class SChronicle_DialogueGraphPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SChronicle_DialogueGraphPin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin);
	virtual const FSlateBrush* GetPinIcon() const override;
};