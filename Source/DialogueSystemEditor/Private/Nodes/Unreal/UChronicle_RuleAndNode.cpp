#include "UChronicle_RuleAndNode.h"

#include "Utils/FChronicle_Colors.h"

void UChronicle_RuleAndNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, TEXT("Condition"), NAME_None, nullptr, TEXT("Condition A"));
	CreatePin(EGPD_Input, TEXT("Condition"), NAME_None, nullptr, TEXT("Condition B"));
	CreatePin(EGPD_Output, TEXT("Condition"), NAME_None, nullptr, TEXT("A and B"));
}

FText UChronicle_RuleAndNode::GetTitle() const
{
	return FText::FromString("And");
}

FSlateColor UChronicle_RuleAndNode::GetBackgroundColor() const
{
	return FChronicle_Colors::Condition;
}
