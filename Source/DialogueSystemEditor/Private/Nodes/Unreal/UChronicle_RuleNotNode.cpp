#include "UChronicle_RuleNotNode.h"

#include "Utils/FChronicle_Colors.h"

void UChronicle_RuleNotNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, TEXT("Condition"), NAME_None, nullptr, TEXT("In"));
	CreatePin(EGPD_Output, TEXT("Condition"), NAME_None, nullptr, TEXT("Out"));
}

FText UChronicle_RuleNotNode::GetTitle() const
{
	return FText::FromString("Not");
}

FSlateColor UChronicle_RuleNotNode::GetBackgroundColor() const
{
	return FChronicle_Colors::Condition;
}
