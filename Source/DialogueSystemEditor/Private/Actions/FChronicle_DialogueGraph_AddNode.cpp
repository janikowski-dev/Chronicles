#include "FChronicle_DialogueGraph_AddNode.h"
#include "EdGraph/EdGraph.h"
#include "Nodes/Unreal/UChronicle_DialogueLineNode.h"
#include "Graphs/UChronicle_DialogueGraph.h"

UEdGraphNode* FChronicle_DialogueGraph_AddNode::PerformAction(
	UEdGraph* ParentGraph,
	UEdGraphPin* FromPin,
	const FVector2f& Location,
	const bool bSelectNewNode
)
{
	UEdGraphNode* NewNode = CreateNewNode(ParentGraph);
	TryAttachingPin(ParentGraph, NewNode, FromPin);
	Cast<UChronicle_DialogueGraph>(ParentGraph)->Refresh();
	return NewNode;
}

UEdGraphNode* FChronicle_DialogueGraph_AddNode::CreateNewNode(UEdGraph* ParentGraph) const
{
	ParentGraph->Modify();

	UEdGraphNode* NewNode = NewObject<UEdGraphNode>(
		ParentGraph,
		NodeClass,
		NAME_None,
		RF_Transactional
	);

	ParentGraph->AddNode(NewNode, true, true);
	NewNode->CreateNewGuid();
	NewNode->PostPlacedNewNode();
	NewNode->AllocateDefaultPins();
	return NewNode;
}

void FChronicle_DialogueGraph_AddNode::TryAttachingPin(
	const UEdGraph* ParentGraph,
	UEdGraphNode* NewNode,
	UEdGraphPin* FromPin
) const
{
	if (!FromPin)
	{
		return;
	}
	
	UEdGraphPin* TargetPin = nullptr;

	for (UEdGraphPin* Pin : NewNode->Pins)
	{
		if (Pin->Direction == FromPin->Direction || Pin->PinType.PinCategory != FromPin->PinType.PinCategory)
		{
			continue;
		}
		
		TargetPin = Pin;
		break;
	}

	if (!TargetPin)
	{
		return;
	}

	ParentGraph->GetSchema()->TryCreateConnection(
		FromPin,
		TargetPin
	);

	const UEdGraphNode* OwnerNode = FromPin->GetOwningNode();
	
	if (NewNode->IsA<UChronicle_DialogueLineNode>() && FromPin->LinkedTo.Num() > 1)
	{
		FromPin->Modify();

		TArray<UEdGraphPin*>& Links = FromPin->LinkedTo;
		int32 NewIndex = INDEX_NONE;

		for (int32 i = 0; i < Links.Num(); ++i)
		{
			if (!Links[i] || Links[i]->GetOwningNode() != NewNode)
			{
				continue;
			}
			
			NewIndex = i;
			break;
		}

		if (NewIndex != INDEX_NONE)
		{
			UEdGraphPin* NewLink = Links[NewIndex];
			Links.RemoveAt(NewIndex);
			Links.Insert(NewLink, 0);
		}

		OwnerNode->GetGraph()->NotifyGraphChanged();
	}
}
