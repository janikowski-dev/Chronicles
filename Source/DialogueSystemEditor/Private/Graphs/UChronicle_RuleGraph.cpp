#include "UChronicle_RuleGraph.h"

#include "Nodes/Unreal/UChronicle_RuleOutputNode.h"

TArray<UChronicle_RuleNode*> UChronicle_RuleGraph::GetRules(const EOutputType Type) const
{
	UChronicle_RuleNode* OutputNode = GetOutputNode(Type);
	TArray<UChronicle_RuleNode*> Rules;
	GetRules(OutputNode, Rules);
	return Rules;
}

UChronicle_RuleNode* UChronicle_RuleGraph::GetOutputNode(const EOutputType Type) const
{
	UEdGraphNode* MatchingNode = Nodes.FindByPredicate([Type](const TObjectPtr<UEdGraphNode> Node)
	{
		const UChronicle_RuleOutputNode* OutputNode = Cast<UChronicle_RuleOutputNode>(Node.Get());

		if (!OutputNode)
		{
			return false;
		}
		
		return OutputNode->Type == Type;
	})->Get();

	return Cast<UChronicle_RuleNode>(MatchingNode);
}

void UChronicle_RuleGraph::GetRules(UChronicle_RuleNode* Node, TArray<UChronicle_RuleNode*>& Rules) const
{
	if (!Node)
	{
		return;
	}

	Rules.Add(Node);

	for (UEdGraphPin* Pin : Node->Pins)
	{
		if (Pin->Direction != EGPD_Input)
		{
			continue;
		}
		
		for (const UEdGraphPin* LinkedPin : Pin->LinkedTo)
		{
			UEdGraphNode* PinOwner = LinkedPin->GetOwningNode();
			UChronicle_RuleNode* TypedOwner = Cast<UChronicle_RuleNode>(PinOwner);
			GetRules(TypedOwner, Rules);
		}
	}
}