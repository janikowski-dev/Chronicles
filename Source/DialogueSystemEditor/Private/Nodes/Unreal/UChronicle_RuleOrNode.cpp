#include "UChronicle_RuleOrNode.h"

#include "Utils/FChronicle_Colors.h"

void UChronicle_RuleOrNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, TEXT("Condition"), NAME_None, nullptr, TEXT("Condition A"));
	CreatePin(EGPD_Input, TEXT("Condition"), NAME_None, nullptr, TEXT("Condition B"));
	CreatePin(EGPD_Output, TEXT("Condition"), NAME_None, nullptr, TEXT("A or B"));
}

FText UChronicle_RuleOrNode::GetTitle() const
{
	return FText::FromString("Or");
}

FSlateColor UChronicle_RuleOrNode::GetBackgroundColor() const
{
	return FChronicle_Colors::Condition;
}
