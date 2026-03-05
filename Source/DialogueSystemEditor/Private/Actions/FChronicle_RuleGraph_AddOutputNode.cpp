#include "FChronicle_RuleGraph_AddOutputNode.h"

#include "EdGraph/EdGraph.h"
#include "Nodes/Unreal/UChronicle_RuleOutputNode.h"

UEdGraphNode* FChronicle_RuleGraph_AddOutputNode::PerformAction(
	UEdGraph* ParentGraph,
	UEdGraphPin*,
	const FVector2f& Location,
	const bool
)
{
	return CreateNewNode(ParentGraph, Location);
}

UEdGraphNode* FChronicle_RuleGraph_AddOutputNode::CreateNewNode(UEdGraph* ParentGraph, const FVector2f& Location) const
{
	ParentGraph->Modify();

	UChronicle_RuleOutputNode* NewNode = NewObject<UChronicle_RuleOutputNode>(
		ParentGraph,
		NodeClass,
		NAME_None,
		RF_Transactional
	);

	NewNode->NodePosX = Location.X;
	NewNode->NodePosY = Location.Y;
	NewNode->Type = OutputType;
	
	NewNode->CreateNewGuid();
	NewNode->PostPlacedNewNode();
	NewNode->AllocateDefaultPins();
	
	ParentGraph->AddNode(NewNode, true, true);
	
	return NewNode;
}
