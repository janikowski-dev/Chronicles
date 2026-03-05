#include "SChronicle_DialogueGraphPin.h"

#include "SGraphPin.h"

void SChronicle_DialogueGraphPin::Construct(const FArguments&, UEdGraphPin* InPin)
{
	SGraphPin::Construct(SGraphPin::FArguments(), InPin);
	SetShowLabel(false);
	SetEnabled(false);
}

const FSlateBrush* SChronicle_DialogueGraphPin::GetPinIcon() const
{
	return nullptr;
}
