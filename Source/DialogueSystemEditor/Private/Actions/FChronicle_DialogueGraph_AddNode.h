#pragma once

struct FChronicle_DialogueGraph_AddNode : FEdGraphSchemaAction
{
	TSubclassOf<UEdGraphNode> NodeClass;

	explicit FChronicle_DialogueGraph_AddNode(const TSubclassOf<UEdGraphNode> InNodeClass) : NodeClass(InNodeClass)
	{
	}

	explicit FChronicle_DialogueGraph_AddNode(
		const FText& InCategory,
		const FText& InMenuDesc,
		const FText& InTooltip,
		const TSubclassOf<UEdGraphNode> InNodeClass
	)
		: FEdGraphSchemaAction(InCategory, InMenuDesc, InTooltip, 0)
		, NodeClass(InNodeClass)
	{}

	virtual UEdGraphNode* PerformAction(
		UEdGraph* ParentGraph,
		UEdGraphPin* FromPin,
		const FVector2f& Location,
		bool bSelectNewNode = true
	) override;
	
	UEdGraphNode* CreateNewNode(UEdGraph* ParentGraph) const;

	void TryAttachingPin(
		const UEdGraph* ParentGraph,
		UEdGraphNode* NewNode,
		UEdGraphPin* FromPin
	) const;
};