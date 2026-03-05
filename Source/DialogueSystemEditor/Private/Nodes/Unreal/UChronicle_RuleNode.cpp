#include "UChronicle_RuleNode.h"

void UChronicle_RuleNode::PostPlacedNewNode()
{
	Super::PostPlacedNewNode();
	AssignId();
}

bool UChronicle_RuleNode::CanUserDeleteNode() const
{
	return true;
}

bool UChronicle_RuleNode::AllowMultipleInputs() const
{
	return false;
}

FText UChronicle_RuleNode::GetTitle() const
{
	return FText::GetEmpty();
}

FSlateColor UChronicle_RuleNode::GetBackgroundColor() const
{
	return FSlateColor();
}

void UChronicle_RuleNode::AssignId()
{
	if (Id.IsValid())
	{
		return;
	}
	
	Id = FGuid::NewGuid();
}
