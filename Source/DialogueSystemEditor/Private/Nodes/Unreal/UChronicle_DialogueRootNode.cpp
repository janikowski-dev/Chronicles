#include "UChronicle_DialogueRootNode.h"

void UChronicle_DialogueRootNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Output, TEXT("DialoguePin"), NAME_None, nullptr, TEXT("Out"));
}

FText UChronicle_DialogueRootNode::GetTitle() const
{
	return FText::FromString("Root #0");
}
