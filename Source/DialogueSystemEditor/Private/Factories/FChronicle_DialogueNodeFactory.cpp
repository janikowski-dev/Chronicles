#pragma once

#include "FChronicle_DialogueNodeFactory.h"
#include "Nodes/Slate/SChronicle_DialogueLineNode.h"
#include "Nodes/Slate/SChronicle_DialogueLinkNode.h"
#include "Nodes/Slate/SChronicle_DialogueRootNode.h"
#include "Nodes/Slate/SChronicle_DialogueResponseNode.h"
#include "Nodes/Unreal/UChronicle_DialogueLineNode.h"
#include "Nodes/Unreal/UChronicle_DialogueLinkNode.h"
#include "Nodes/Unreal/UChronicle_DialogueResponseNode.h"
#include "Nodes/Unreal/UChronicle_DialogueRootNode.h"

TSharedPtr<SGraphNode> FChronicle_DialogueNodeFactory::CreateNode(UEdGraphNode* Node) const
{
	if (UChronicle_DialogueLineNode* TypedNode = Cast<UChronicle_DialogueLineNode>(Node))
	{
		return SNew(SChronicle_DialogueLineNode, TypedNode);
	}
		
	if (UChronicle_DialogueRootNode* TypedNode = Cast<UChronicle_DialogueRootNode>(Node))
	{
		return SNew(SChronicle_DialogueRootNode, TypedNode);
	}
		
	if (UChronicle_DialogueResponseNode* TypedNode = Cast<UChronicle_DialogueResponseNode>(Node))
	{
		return SNew(SChronicle_DialogueResponseNode, TypedNode);
	}
		
	if (UChronicle_DialogueLinkNode* TypedNode = Cast<UChronicle_DialogueLinkNode>(Node))
	{
		return SNew(SChronicle_DialogueLinkNode, TypedNode);
	}
		
	return nullptr;
}
