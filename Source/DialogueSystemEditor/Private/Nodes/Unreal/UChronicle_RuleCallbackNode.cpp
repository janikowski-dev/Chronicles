#include "UChronicle_RuleCallbackNode.h"

#include "Utils/FChronicle_Colors.h"

void UChronicle_RuleCallbackNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Output, TEXT("Callback"), NAME_None, nullptr, TEXT("Output"));
}

FText UChronicle_RuleCallbackNode::GetTitle() const
{
	return FText::FromString("Callback");
}

FSlateColor UChronicle_RuleCallbackNode::GetBackgroundColor() const
{
	return FChronicle_Colors::Callback;
}
