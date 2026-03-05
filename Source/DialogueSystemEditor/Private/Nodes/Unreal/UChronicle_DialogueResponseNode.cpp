#include "UChronicle_DialogueResponseNode.h"

void UChronicle_DialogueResponseNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input,  TEXT("DialoguePin"), NAME_None, nullptr, TEXT("In"));
	CreatePin(EGPD_Output, TEXT("DialoguePin"), NAME_None, nullptr, TEXT("Out"));
}

FText UChronicle_DialogueResponseNode::GetTitle() const
{
	return FText::Format(FText::FromString("Player Response #{0}.{1}"), ParentIndex, OrderIndex);
}

FText UChronicle_DialogueResponseNode::GetText() const
{
	return Text;
}
