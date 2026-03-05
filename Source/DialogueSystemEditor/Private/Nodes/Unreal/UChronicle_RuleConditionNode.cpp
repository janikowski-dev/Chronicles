#include "UChronicle_RuleConditionNode.h"

#include "Utils/FChronicle_Colors.h"

void UChronicle_RuleConditionNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Output, TEXT("Condition"), NAME_None, nullptr, TEXT("Value"));
}

FText UChronicle_RuleConditionNode::GetTitle() const
{
	return FText::FromString("Condition");
}

FSlateColor UChronicle_RuleConditionNode::GetBackgroundColor() const
{
	return FChronicle_Colors::Condition;
}
