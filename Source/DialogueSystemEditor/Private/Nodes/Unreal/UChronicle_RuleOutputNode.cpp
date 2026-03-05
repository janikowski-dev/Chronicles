#include "UChronicle_RuleOutputNode.h"

#include "Utils/FChronicle_Colors.h"

void UChronicle_RuleOutputNode::AllocateDefaultPins()
{
	if (Type == EOutputType::Requirements)
	{
		CreatePin(EGPD_Input, TEXT("Condition"), NAME_None, nullptr, TEXT("Result"));
	}
	else
	{
		CreatePin(EGPD_Input, TEXT("Callback"), NAME_None, nullptr, TEXT("Result"));
	}
}

bool UChronicle_RuleOutputNode::AllowMultipleInputs() const
{
	return Type != EOutputType::Requirements;
}

FText UChronicle_RuleOutputNode::GetTitle() const
{
	switch (Type)
	{
	case EOutputType::PostCallback: return FText::FromString("Post Callbacks");
	case EOutputType::Requirements: return FText::FromString("Availability Requirements");
	default: return FText::FromString("Invalid Node");
	}
}

FSlateColor UChronicle_RuleOutputNode::GetBackgroundColor() const
{
	if (Type == EOutputType::Requirements)
	{
		return FChronicle_Colors::Condition;
	}
	
	return FChronicle_Colors::Callback;
}

bool UChronicle_RuleOutputNode::CanUserDeleteNode() const
{
	return false;
}
