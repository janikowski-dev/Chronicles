#include "UChronicle_DialogueLinkNode.h"

#include "UChronicle_DialogueLineNode.h"

void UChronicle_DialogueLinkNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, TEXT("DialoguePin"), NAME_None, nullptr, TEXT("In"));
}

FText UChronicle_DialogueLinkNode::GetTitle() const
{
	if (LinkedNode)
	{
		return FText::Format(FText::FromString("Link To Line #{0}"), LinkedNode->LineIndex);
	}
	
	return FText::FromString("Empty Link");
}

UChronicle_DialogueLineNode* UChronicle_DialogueLinkNode::GetLinkedNode() const
{
	return LinkedNode;
}

void UChronicle_DialogueLinkNode::SetLinkedNode(UChronicle_DialogueLineNode* InNode)
{
	LinkedNode = InNode;
}
