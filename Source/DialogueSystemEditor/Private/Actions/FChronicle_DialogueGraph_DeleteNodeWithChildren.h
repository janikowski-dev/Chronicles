#pragma once

struct FChronicle_DialogueGraph_DeleteNodeWithChildren : FEdGraphSchemaAction
{
	virtual UEdGraphNode* PerformAction(
		UEdGraph* ParentGraph,
		UEdGraphPin* FromPin,
		const FVector2f& Location,
		bool bSelectNewNode = true
	) override;
	
	UEdGraphNode* GetNode(const UEdGraphPin* Pin) const;
	void DestroyWithChildren(UEdGraphNode* Node) const;
};