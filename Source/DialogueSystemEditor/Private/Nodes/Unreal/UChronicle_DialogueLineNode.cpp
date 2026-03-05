#include "UChronicle_DialogueLineNode.h"

void UChronicle_DialogueLineNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input,  TEXT("DialoguePin"), NAME_None, nullptr, TEXT("In"));
	CreatePin(EGPD_Output, TEXT("DialoguePin"), NAME_None, nullptr, TEXT("Out"));
}

FText UChronicle_DialogueLineNode::GetTitle() const
{
	return  FText::Format(FText::FromString("Line #{0}"), LineIndex);
}

FText UChronicle_DialogueLineNode::GetText() const
{
	return Text;
}
