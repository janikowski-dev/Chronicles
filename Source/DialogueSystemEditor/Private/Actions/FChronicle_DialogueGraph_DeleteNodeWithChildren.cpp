#include "FChronicle_DialogueGraph_DeleteNodeWithChildren.h"
#include "EdGraph/EdGraph.h"
#include "Graphs/UChronicle_DialogueGraph.h"

UEdGraphNode* FChronicle_DialogueGraph_DeleteNodeWithChildren::PerformAction(
	UEdGraph* ParentGraph,
	UEdGraphPin* FromPin,
	const FVector2f&,
	const bool
)
{
	if (UEdGraphNode* Node = GetNode(FromPin))
	{
		DestroyWithChildren(Node);
	}
	
	Cast<UChronicle_DialogueGraph>(ParentGraph)->Refresh();
	return nullptr;
}

UEdGraphNode* FChronicle_DialogueGraph_DeleteNodeWithChildren::GetNode(const UEdGraphPin* Pin) const
{
	UEdGraphNode* Node = Cast<UEdGraphNode>(Pin->GetOwningNode());

	if (!Node)
	{
		return nullptr;
	}

	if (Node->CanUserDeleteNode())
	{
		return Node;
	}

	return nullptr;
}

void FChronicle_DialogueGraph_DeleteNodeWithChildren::DestroyWithChildren(UEdGraphNode* Node) const
{
	if (!Node)
	{
		return;
	}

	Node->Modify();

	TArray<UEdGraphNode*> ChildNodes;
	
	for (UEdGraphPin* Pin : Node->Pins)
	{
		if (Pin->Direction != EGPD_Output)
		{
			continue;
		}
		
		for (const UEdGraphPin* LinkedPin : Pin->LinkedTo)
		{
			UEdGraphNode* ChildNode = LinkedPin->GetOwningNode();
			
			if (!ChildNode)
			{
				continue;
			}
			
			ChildNodes.AddUnique(ChildNode);
		}
	}

	for (UEdGraphNode* ChildNode : ChildNodes)
	{
		DestroyWithChildren(ChildNode);
	}

	Node->DestroyNode();
}
