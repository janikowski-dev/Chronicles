#include "UChronicle_DialogueNode.h"

#include "Assets/UChronicle_DialogueAsset.h"
#include "Graphs/UChronicle_RuleGraph.h"
#include "Schemas/UChronicle_RuleGraphSchema.h"

void UChronicle_DialogueNode::PostPlacedNewNode()
{
	Super::PostPlacedNewNode();
	AssignId();
}

FText UChronicle_DialogueNode::GetTitle() const
{
	return FText::GetEmpty();
}

FText UChronicle_DialogueNode::GetText() const
{
	return FText::GetEmpty();
}

UChronicle_RuleGraph* UChronicle_DialogueNode::GetOrCreateInnerGraph()
{
	if (UChronicle_DialogueAsset* Asset = GetTypedOuter<UChronicle_DialogueAsset>())
	{
		if (const TObjectPtr<UChronicle_RuleGraph>* Found = Asset->InnerGraphsByNode.Find(this))
		{
			return Found->Get();
		}

		UChronicle_RuleGraph* InnerGraph = NewObject<UChronicle_RuleGraph>(
			Asset,
			UChronicle_RuleGraph::StaticClass(),
			NAME_None,
			RF_Transactional
		);
	
		InnerGraph->Schema = UChronicle_RuleGraphSchema::StaticClass();
		InnerGraph->GetSchema()->CreateDefaultNodesForGraph(*InnerGraph);
		Asset->InnerGraphsByNode.Add(this, InnerGraph);
		return InnerGraph;
	}

	return nullptr;
}

UChronicle_RuleGraph* UChronicle_DialogueNode::GetInnerGraph() const
{
	if (UChronicle_DialogueAsset* Asset = GetTypedOuter<UChronicle_DialogueAsset>())
	{
		if (const TObjectPtr<UChronicle_RuleGraph>* Found = Asset->InnerGraphsByNode.Find(this))
		{
			return Found->Get();
		}
	}

	return nullptr;
}

void UChronicle_DialogueNode::AssignId()
{
	if (Id.IsValid())
	{
		return;
	}
	
	Id = FGuid::NewGuid();
}
