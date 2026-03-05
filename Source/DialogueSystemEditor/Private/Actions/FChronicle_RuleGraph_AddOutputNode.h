#pragma once

enum class EOutputType : int8;

struct FChronicle_RuleGraph_AddOutputNode : FEdGraphSchemaAction
{
	TSubclassOf<UEdGraphNode> NodeClass;
	EOutputType OutputType;

	explicit FChronicle_RuleGraph_AddOutputNode(
		const TSubclassOf<UEdGraphNode> InNodeClass,
		const EOutputType OutputType)
	: NodeClass(InNodeClass), OutputType(OutputType)
	{
	}

	virtual UEdGraphNode* PerformAction(
		UEdGraph* ParentGraph,
		UEdGraphPin* FromPin,
		const FVector2f& Location,
		bool bSelectNewNode = true
	) override;
	
	UEdGraphNode* CreateNewNode(UEdGraph* ParentGraph, const FVector2f& Location) const;
};
